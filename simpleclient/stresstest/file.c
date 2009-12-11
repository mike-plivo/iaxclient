#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "file.h"

#ifdef __GNUC__
void mylog(const char * fmt, ...) __attribute__ ((format (printf, 1, 2)));
#else
void mylog(const char * fmt, ...);
#endif

static struct ogg_stream *audio_stream;
static struct ogg_stream *video_stream;

struct op_node * create_node(ogg_packet *op, long serialno, long timestamp)
{
	struct op_node *node;

	node = malloc(sizeof(struct op_node));
	node->timestamp = timestamp;
	node->serialno = serialno;
	node->op = malloc(sizeof(*op));
	memcpy(node->op, op, sizeof(*op));
	node->op->packet = malloc(op->bytes);
	memcpy(node->op->packet, op->packet, op->bytes);

	return node;
}

void append_node(struct ogg_stream *os, struct op_node *node)
{
	if ( os->first == NULL )
	{
		if ( os->last != NULL )
		{
			mylog("Queue inconsistency, bailing...\n");
			return;
		}
		os->first = node;
		os->last = node;
		node->next = NULL;
	} else
	{
		if ( os->last == NULL )
		{
			mylog("Queue inconsistency, bailing...\n");
			return;
		}
		os->last->next = node;
		os->last = node;
		node->next = NULL;
	}
}

/*
 * We're forced to use a dirty hack here, due to Theora's idiotic API
 * Theora needs three separate pieces of data, called headers to initialize
 * its internal decoder structure.  After all three pieces have been received,
 * we can call theora_decode_init.
 * We use a counter and a flag to make sure we have decoded our three headers and then
 * we call theora_decode_init so we can initialize a theora_state structure.
 * We use the ts structure to convert a granule position into an actual timestamp.
 * There are many ways in which this can fail, but we rely on having all three headers
 * at the beginning of the ogg video bitstream.
 *
 * To whoever came up with this convoluted scheme: please consider a change of careers.
 */
int read_theora_cb(OGGZ *oggz, ogg_packet *op, long serialno, void *data)
{
	struct op_node        *node;
	struct theora_headers *th;
	long                  timestamp = 0;

	//mylog("Got theora packet, serialno=%d, size=%d, packetno=%lld, granulepos=%lld\n",
	//		serialno, op->bytes, op->packetno, op->granulepos);

	th = (struct theora_headers *)video_stream->data;

	if ( theora_packet_isheader(op) )
	{
		theora_decode_header(&(th->ti), &(th->tc), op);
		th->header_count++;
	}

	if ( th->header_count >= 3 && !th->have_headers )
	{
		theora_decode_init(&(th->ts), &(th->ti));
		th->have_headers = 1;
	}

	if ( th->have_headers )
	{
		double d;

		d = theora_granule_time(&(th->ts), op->granulepos);
		timestamp = (long)(d * 1000);
	}

	if ( timestamp < 0 )
	{
		timestamp = video_stream->page_ts + video_stream->page_count * THEORA_FRAME_DURATION;
		video_stream->page_count++;
	} else
	{
		video_stream->page_ts = timestamp;
		video_stream->page_count = 0;
	}

	if ( !theora_packet_isheader(op) )
	{
		node = create_node(op, serialno, timestamp);
		append_node(video_stream, node);
	}

	return 0;
}

int read_speex_cb(OGGZ *oggz, ogg_packet *op, long serialno, void *data)
{
	struct op_node *node;
	long            timestamp;
	static int      cnt = 0;

	timestamp = audio_stream->page_ts + audio_stream->page_count * SPEEX_FRAME_DURATION;
	audio_stream->page_count++;

	cnt++;
	//mylog("Got speex packet, serialno=%ld, size=%ld, packetno=%lld, granulepos=%lld, timestamp=%ld\n",
	//		serialno, op->bytes, op->packetno, op->granulepos, timestamp);

	// Ignore the first two packets, they are headers
	if ( cnt > 2 )
	{
		node = create_node(op, serialno, timestamp);
		append_node(audio_stream, node);
	}

	return 0;
}

int read_cb(OGGZ *oggz, ogg_packet *op, long serialno, void *data)
{
	struct theora_headers *th;

	const char theoraId[] = "\x80theora";
	const char speexId[] = "Speex   ";

	if ( memcmp(op->packet, theoraId, strlen(theoraId)) == 0 )
	{
		//mylog("Detected a Theora stream with serialno=%d\n", serialno);
		oggz_set_read_callback(oggz, serialno, read_theora_cb, NULL);
		video_stream->serialno = serialno;

		// Initialize theora specific data fields
		th = (struct theora_headers *)calloc(1, sizeof(struct theora_headers));
		theora_info_init(&(th->ti));
		theora_comment_init(&(th->tc));
		video_stream->data = th;

		read_theora_cb(oggz, op, serialno, data);
	} else if ( memcmp(op->packet, speexId, strlen(speexId)) == 0 )
	{
		//mylog("Detected a Speex stream with serialno=%d\n", serialno);
		oggz_set_read_callback(oggz, serialno, read_speex_cb, NULL);
		audio_stream->serialno = serialno;
		read_speex_cb(oggz, op, serialno, data);
	} else
	{
		mylog("Got unknown ogg packet, serialno=%d, size=%d, packetno=%d, granulepos=%d\n",
				serialno, op->bytes, op->packetno, op->granulepos);
	}
	return 0;
}

int read_page_cb(OGGZ *oggz, const ogg_page *og, long serialno, void *data)
{
	if ( serialno == audio_stream->serialno )
	{
		audio_stream->page_ts = ogg_page_granulepos(og) * 1000 / SPEEX_SAMPLING_RATE;
		audio_stream->page_count = 0;
	} else if ( serialno == video_stream->serialno )
	{
		//mylog("Got theora page serialno=%d, header_len=%d, body_len=%d, granulepos=%lld\n",
		//		serialno, og->header_len, og->body_len, ogg_page_granulepos(og));
	}
	return 0;
}

void dump_stream(struct ogg_stream *os)
{
	struct op_node *node;

	node = os->first;
	while ( node != NULL )
	{
		mylog("Size=%ld, Stream=%ld, packetno=%lld, timestamp=%ld\n",
				node->op->bytes, node->serialno,
				node->op->packetno, node->timestamp);
		node = node->next;
	}
}

void load_ogg_file(const char *filename)
{
	OGGZ *oggz;

	oggz = oggz_open(filename, OGGZ_READ | OGGZ_AUTO);
	if ( oggz == NULL )
	{
		mylog("Error opening ogg file\n");
	}
	mylog("Successfully opened ogg file %s\n", filename);

	// Initialize internal streams
	audio_stream = calloc(1, sizeof(struct ogg_stream));
	video_stream = calloc(1, sizeof(struct ogg_stream));

	oggz_set_read_callback(oggz, -1, read_cb, NULL);
	oggz_set_read_page(oggz, -1, read_page_cb, NULL);

	oggz_run(oggz);

	//mylog("Audio stream, serialno=%d\n", audio_stream->serialno);
	//dump_stream(audio_stream);
	//mylog("Video stream, serialno=%d\n", video_stream->serialno);
	//dump_stream(video_stream);

	oggz_close(oggz);
}

ogg_packet * get_next_op(struct ogg_stream *os)
{
	ogg_packet     *op;
	struct timeval tv;
	long           time_now;

	if ( os == NULL )
		return NULL;

	gettimeofday(&tv, NULL);
	time_now = tv.tv_sec * 1000 + tv.tv_usec / 1000;

	if ( os->current == NULL )
	{
		// point to the beginning of the stream and reset the time base
		os->base_ts = time_now;
		os->current = os->first;
	}

	op = NULL;
	if ( os->current->timestamp < time_now - os->base_ts )
	{
		op = os->current->op;
		os->current = os->current->next;
	}

	return op;
}

ogg_packet * get_next_audio_op()
{
	return get_next_op(audio_stream);
}

ogg_packet * get_next_video_op()
{
	return get_next_op(video_stream);
}

int audio_is_eos()
{
	return audio_stream->current == NULL;
}

int video_is_eos()
{
	return video_stream->current == NULL;
}


#ifndef __FILE_H__
#define __FILE_H__

#include <oggz/oggz.h>
#include <theora/theora.h>

#define SPEEX_FRAME_DURATION  20
#define SPEEX_SAMPLING_RATE   8000

#define THEORA_FRAME_DURATION 1000 / 15

// Struct used to build chains of packets for delivery
struct op_node
{
	ogg_packet     *op;
	long           serialno;
	long           timestamp;
	struct op_node *next;
};

struct ogg_stream
{
	struct op_node *first;
	struct op_node *last;
	struct op_node *current;
	long           serialno;
	long           page_ts;
	long           page_count;
	long           base_ts;
	void           *data;
};

struct theora_headers
{
	theora_info    ti;
	theora_comment tc;
	theora_state   ts;
	int            header_count;
	int            have_headers;
};

int read_theora_cb(OGGZ *oggz, ogg_packet *op, long serialno, void *data);
int read_speex_cb(OGGZ *oggz, ogg_packet *op, long serialno, void *data);
int read_cb(OGGZ *oggz, ogg_packet *op, long serialno, void *data);
int read_page_cb(OGGZ *oggz, const ogg_page *og, long serialno, void *data);
void load_ogg_file(const char *filename);

ogg_packet * get_next_audio_op();
ogg_packet * get_next_video_op();

int audio_is_eos();
int video_is_eos();

#endif // __FILE_H__

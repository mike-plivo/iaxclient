using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace IAX.Client
{
    // Define audio type constants
    public enum IAXAudioType
    {
        Internal = 0, InternalPA = 1, InternalFile = 2, InternalAlsa = 3, External = 99
    }

    // Data formats for capabilities and frames alike 
    [Flags]
    public enum IAXFormat
    {
        G723_1 = 1 << 0, GSM = 1 << 1, ULAW = 1 << 2, ALAW = 1 << 3, G726 = 1 << 4, ADPCM = 1 << 5,
        SLINEAR = 1 << 6, LPC10 = 1 << 7, G729A = 1 << 8, SPEEX = 1 << 9, ILBC = 1 << 10, MAX_AUDIO = 1 << 15,
        JPEG = 1 << 16, PNG = 1 << 17, H261 = 1 << 18, H263 = 1 << 19, H263_PLUS = 1 << 20, MPEG4 = 1 << 21,
        H264 = 1 << 23, THEORA = 1 << 24
    }

    [Flags]
    public enum IAXCallState
    {
        Free = 0, Active = 1 << 1, Outgoing = 1 << 2, Ringing = 1 << 3, Complete = 1 << 4,
        Selected = 1 << 5, Busy = 1 << 6, Transfer = 1 << 7
    }

    public enum IAXTTextEventType
    {
        Status = 1, Notice = 2, Error = 3, FatalError = 4, IAX = 5        // FATAL ERROR: User Agent should probably display error, then die. 
    }

    public enum IAXRegistrationReply
    {
        Ack = 18,       // IAX_EVENT_REGACC 
        Reject = 30,    // IAX_EVENT_REGREJ
        Timeout = 6     // IAX_EVENT_TIMEOUT
    }

    public enum IAXUrlEventType
    {
        Url = 1,            // URL received 
        LoadComplete = 2,   // URL loading complete 
        LunkUrl = 3,        // URL link request
        LinkReject = 4,     // URL link reject 
        Unlink = 5          // URL unlink 
    }

    [Flags]
    public enum IAXFilter
    {
        IAXC_FILTER_DENOISE = 1 << 0,
        IAXC_FILTER_AGC = 1 << 1,
        IAXC_FILTER_ECHO = 1 << 2,
        IAXC_FILTER_AAGC = 1 << 3, // Analog (mixer-based) AGC
        IAXC_FILTER_CN = 1 << 4    // Send CN frames when silence detected
    }

    // this class declares the unmanaged types & functions from iaxclient.dll
    internal class IAXUnmanaged
    {
        internal const int IAXC_EVENT_TEXT = 1;
        internal const int IAXC_EVENT_LEVELS = 2;
        internal const int IAXC_EVENT_STATE = 3;
        internal const int IAXC_EVENT_NETSTAT = 4;
        internal const int IAXC_EVENT_URL = 5;	// URL push via IAX(2) 
        internal const int IAXC_EVENT_VIDEO = 6;	// video data (pointer) 
        internal const int IAXC_EVENT_REGISTRATION = 7;

        internal const int IAXC_EVENT_BUFSIZ = 256;

        //struct iaxc_ev_levels {
        // float input;
        // float output;
        //};
        [StructLayout(LayoutKind.Sequential)]
        internal struct iaxc_ev_levels
        {
            internal float input;
            internal float output;
        }

        //struct iaxc_ev_text {
        // int type;
        // int callNo; /* call number for IAX text */
        // char message[IAXC_EVENT_BUFSIZ];
        //};
        [StructLayout(LayoutKind.Sequential)]
        internal struct iaxc_ev_text
        {
            internal int type;
            internal int callNo;
            internal unsafe fixed byte message[IAXC_EVENT_BUFSIZ];
        }

        //struct iaxc_ev_call_state {
        // int callNo;
        // int state;
        // int format;
        // char remote[IAXC_EVENT_BUFSIZ];
        // char remote_name[IAXC_EVENT_BUFSIZ];
        // char local[IAXC_EVENT_BUFSIZ];
        // char local_context[IAXC_EVENT_BUFSIZ];
        //};
        [StructLayout(LayoutKind.Sequential)]
        internal struct iaxc_ev_call_state
        {
            internal int callNo;
            internal int state;
            internal int format;
            internal unsafe fixed byte remote[IAXC_EVENT_BUFSIZ];
            internal unsafe fixed byte remote_name[IAXC_EVENT_BUFSIZ];
            internal unsafe fixed byte local[IAXC_EVENT_BUFSIZ];
            internal unsafe fixed byte local_context[IAXC_EVENT_BUFSIZ];
        }

        //struct iaxc_netstat {
        //        int jitter;
        //        int losspct;
        //        int losscnt;
        //        int packets;
        //        int delay;
        //        int dropped;
        //        int ooo;
        //};
        [StructLayout(LayoutKind.Sequential)]
        internal struct iaxc_netstat
        {
            internal int jitter;
            internal int losspct;
            internal int losscnt;
            internal int packets;
            internal int delay;
            internal int dropped;
            internal int ooo;
        }

        //struct iaxc_ev_netstats {
        // int callNo;
        // int rtt;
        // struct iaxc_netstat local;
        // struct iaxc_netstat remote;
        //};
        [StructLayout(LayoutKind.Sequential)]
        internal struct iaxc_ev_netstats
        {
            internal int callNo;
            internal int rtt;
            internal iaxc_netstat local;
            internal iaxc_netstat remote;
        }

        //struct iaxc_ev_url {
        // int callNo;
        // int type;
        // char url[IAXC_EVENT_BUFSIZ];
        //};
        [StructLayout(LayoutKind.Sequential)]
        internal struct iaxc_ev_url
        {
            internal int callNo;
            internal int type;
            internal unsafe fixed byte url[IAXC_EVENT_BUFSIZ];
        }

        //struct iaxc_ev_video {
        // int callNo;
        // int format;
        // int width;
        // int height;
        // unsigned char *data;
        //};
        [StructLayout(LayoutKind.Sequential)]
        internal unsafe struct iaxc_ev_video
        {
            internal int callNo;
            internal int format;
            internal int width;
            internal int height;
            internal void* data;
        }

        //struct iaxc_ev_registration {
        //    int id;
        //    int reply;
        //    int msgcount;
        //};
        [StructLayout(LayoutKind.Sequential)]
        internal struct iaxc_ev_registration
        {
            internal int id;
            internal int reply;
            internal int msgcount;
        }

        //typedef struct iaxc_event_struct {
        // struct iaxc_event_struct *next;
        // int type;
        // union {
        //  struct iaxc_ev_levels   levels;
        //  struct iaxc_ev_text   text;
        //  struct iaxc_ev_call_state  call;
        //  struct iaxc_ev_netstats  netstats;
        //  struct iaxc_ev_url          url;
        //  struct iaxc_ev_video  video;
        //  struct iaxc_ev_registration reg;
        // } ev;
        //} iaxc_event;
        [StructLayout(LayoutKind.Explicit)]
        internal unsafe struct iaxc_event
        {
            [FieldOffset(0)]
            internal iaxc_event* next;
            [FieldOffset(4)]
            internal int type;
            [FieldOffset(8)]
            internal iaxc_ev_levels levels;
            [FieldOffset(8)]
            internal iaxc_ev_text text;
            [FieldOffset(8)]
            internal iaxc_ev_call_state call;
            [FieldOffset(8)]
            internal iaxc_ev_netstats netstats;
            [FieldOffset(8)]
            internal iaxc_ev_url url;
            [FieldOffset(8)]
            internal iaxc_ev_video video;
            [FieldOffset(8)]
            internal iaxc_ev_registration reg;
        }

        //typedef int (*iaxc_event_callback_t)(iaxc_event e);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        internal delegate int iaxc_event_callback_t(iaxc_event e);

        //EXPORT void iaxc_set_event_callback(iaxc_event_callback_t func);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_set_event_callback(iaxc_event_callback_t func);

        ///* Sets iaxclient to post a pointer to a copy of event using o/s specific Post method */
        //EXPORT int iaxc_set_event_callpost(void *handle, int id);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern int iaxc_set_event_callpost(IntPtr handle, int id);

        ///* frees event delivered via o/s specific Post method */
        //EXPORT void iaxc_free_event(iaxc_event *e); 
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_free_event(ref iaxc_event e);

        ///* Event Accessors */
        //EXPORT struct iaxc_ev_levels *iaxc_get_event_levels(iaxc_event *e);
        //EXPORT struct iaxc_ev_text *iaxc_get_event_text(iaxc_event *e);
        //EXPORT struct iaxc_ev_call_state *iaxc_get_event_state(iaxc_event *e);

        //EXPORT void iaxc_set_preferred_source_udp_port(int sourceUdpPort);	//must be called before iaxc_initialize()
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_set_preferred_source_udp_port(int sourceUdpPort);

        //EXPORT int iaxc_initialize(int audType, int nCalls);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern int iaxc_initialize(int audType, int nCalls);

        //EXPORT void iaxc_shutdown();
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_shutdown();

        //EXPORT void iaxc_set_formats(int preferred, int allowed);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_set_formats(int preferred, int allowed);

        //EXPORT void iaxc_set_min_outgoing_framesize(int samples);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_set_min_outgoing_framesize(int samples);

        //EXPORT void iaxc_set_callerid(char *name, char *number);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_set_callerid(string name, string number);

        //EXPORT void iaxc_process_calls();
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_process_calls();

        //EXPORT int iaxc_service_audio();
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern int iaxc_service_audio();

        //EXPORT int iaxc_start_processing_thread();
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern int iaxc_start_processing_thread();

        //EXPORT int iaxc_stop_processing_thread();
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern int iaxc_stop_processing_thread();

        //EXPORT void iaxc_call(char *num);
        [DllImport("iaxclient.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_call(string num);

        //EXPORT int iaxc_unregister( int id );
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern int iaxc_unregister(int id);

        //EXPORT int iaxc_register(char *user, char *pass, char *host);
        [DllImport("iaxclient.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        internal static extern int iaxc_register(string user, string pass, string host);

        //EXPORT void iaxc_answer_call(int callNo);
        [DllImport("iaxclient.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_answer_call(int callNo);

        //EXPORT void iaxc_blind_transfer_call(int callNo, char *number); 
        [DllImport("iaxclient.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_blind_transfer_call(int callNo, string number);

        //EXPORT void iaxc_dump_all_calls(void);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_dump_all_calls();

        //EXPORT void iaxc_dump_call(void);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_dump_call();

        //EXPORT void iaxc_reject_call(void);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_reject_call();

        //EXPORT void iaxc_reject_call_number(int callNo);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_reject_call_number(int callNo);

        //EXPORT void iaxc_send_dtmf(char digit);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_send_dtmf(char digit);

        //EXPORT void iaxc_send_text(char *text);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_send_text(string text);

        //EXPORT void iaxc_send_url(char *url, int link); /* link == 1 ? AST_HTML_LINKURL : AST_HTML_URL */
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_send_url(string url, int link);

        //EXPORT int iaxc_was_call_answered();
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern int iaxc_was_call_answered();

        //EXPORT void iaxc_millisleep(long ms);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_millisleep(long ms);

        //EXPORT void iaxc_set_silence_threshold(double thr);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_set_silence_threshold(double thr);

        //EXPORT void iaxc_set_audio_output(int mode);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_set_audio_output(int mode);

        //EXPORT int iaxc_select_call(int callNo);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern int iaxc_select_call(int callNo);

        //EXPORT int iaxc_first_free_call();
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern int iaxc_first_free_call();

        //EXPORT int iaxc_selected_call();
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern int iaxc_selected_call();

        //EXPORT int iaxc_quelch(int callNo, int MOH);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern int iaxc_quelch(int callNo, int MOH);

        //EXPORT int iaxc_unquelch(int call);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern int iaxc_unquelch(int call);

        //EXPORT int iaxc_mic_boost_get( void ) ;
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern int iaxc_mic_boost_get();

        //EXPORT int iaxc_mic_boost_set( int enable ) ;
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_mic_boost_set(int enable);

        //EXPORT char* iaxc_version(char *ver);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern string iaxc_version(string ver);

        ///* application-defined networking; give substiture sendto and recvfrom functions,
        // * must be called before iaxc_initialize! */
        //EXPORT void iaxc_set_networking(iaxc_sendto_t st, iaxc_recvfrom_t rf) ;

        ///* wrapper for libiax2 get_netstats */
        //EXPORT int iaxc_get_netstats(int call, int *rtt, struct iaxc_netstat *local, struct iaxc_netstat *remote);

        //#define IAXC_AD_INPUT           (1<<0)
        //#define IAXC_AD_OUTPUT          (1<<1)
        //#define IAXC_AD_RING            (1<<2)
        //#define IAXC_AD_INPUT_DEFAULT   (1<<3)
        //#define IAXC_AD_OUTPUT_DEFAULT  (1<<4)
        //#define IAXC_AD_RING_DEFAULT    (1<<5)

        //struct iaxc_audio_device {
        //    char *name;             /* name of the device */
        //    long capabilities;      /* flags, defined above */
        //    int devID;              /* driver-specific ID */
        //};

        ///* Get audio device information:
        // * 	**devs: a pointer to an array of device structures, as declared above.  function
        // * 	will give you a pointer to the proper array, which will be valid as long as iaxc is
        // * 	initialized.
        // *
        // * 	*nDevs: a pointer to an int, to which the count of devices in the array devs will be
        // * 	written
        // *
        // * 	*input, *output, *ring: the currently selected devices for input, output, ring will
        // * 	be written to the int pointed to by these pointers.
        // */
        //EXPORT int iaxc_audio_devices_get(struct iaxc_audio_device **devs, int *nDevs, int *input, int *output, int *ring); 
        //EXPORT int iaxc_audio_devices_set(int input, int output, int ring);

        //EXPORT double iaxc_input_level_get();
        //EXPORT double iaxc_output_level_get();
        //EXPORT int iaxc_input_level_set(double level);
        //EXPORT int iaxc_output_level_set(double level);


        //struct iaxc_sound {
        //    short 	*data;          /* sound data */
        //    long 	len;            /* length of sample */
        //    int 	malloced;	/* should the library free() the data after it is played? */
        //    int	channel;	/* 0 for outputSelected, 1 for ringSelected */
        //    int 	repeat;      	/* number of times to repeat (-1 = infinite) */
        //    long	pos;		/* internal use: current play position */
        //    int 	id;		/* internal use: sound ID */
        //    struct iaxc_sound *next; /* internal use: next in list */
        //};

        ///* play a sound.  sound = an iaxc_sound structure, ring: 0: play through output device; 1: play through "ring" device */
        //EXPORT int iaxc_play_sound(struct iaxc_sound *sound, int ring);

        ///* stop sound with ID "id" */
        //EXPORT int iaxc_stop_sound(int id);

        //EXPORT int iaxc_get_filters(void);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern IAXFilter iaxc_get_filters();

        //EXPORT void iaxc_set_filters(int filters);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern void iaxc_set_filters(IAXFilter filters);

        //EXPORT int iaxc_set_files(FILE *input, FILE *output);
        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]
        internal static extern int iaxc_set_files(Microsoft.Win32.SafeHandles.SafeFileHandle input, Microsoft.Win32.SafeHandles.SafeFileHandle output);

        ///* speex specific codec settings */
        ///* a good choice is (1,-1,-1,0,8000,3): 8kbps ABR */
        ///* Decode options:
        //    * decode_enhance: 1/0  perceptual enhancement for decoder 
        //    * quality: Generally, set either quality (0-9) or bitrate.  
        //    *    -1 for "default"
        //    * bitrate: in kbps.  Applies to CBR only; -1 for default.  
        //    *    (overrides "quality" for CBR mode)
        //    * vbr: Variable bitrate mode:  0/1
        //    * abr mode/rate:  0 for not ABR, bitrate for ABR mode
        //    * complexity:  algorithmic complexity.  Think -N for gzip.
        //    *    Higher numbers take more CPU for better quality.  3 is
        //    *    default and good choice.
        //*/
        //EXPORT void iaxc_set_speex_settings(int decode_enhance, float quality, int bitrate, int vbr, int abr, int complexity);

        ///* set/get video mode */
        //#define IAXC_VIDEO_MODE_NONE 			0  /* don't send video at all */
        //#define IAXC_VIDEO_MODE_ACTIVE 			1  /* send video */
        //#define IAXC_VIDEO_MODE_PREVIEW_RAW 		2  /* send video, and show raw preview */
        //#define IAXC_VIDEO_MODE_PREVIEW_ENCODED  	3  /* send video, and show encoded preview */
        //EXPORT int iaxc_video_mode_set(int mode);
        //EXPORT int iaxc_video_mode_get();

        ///* set allowed/preferred video encodings */
        //EXPORT void iaxc_video_format_set(int preferred, int allowed, int framerate, int bitrate, int width, int height);    
    }
}

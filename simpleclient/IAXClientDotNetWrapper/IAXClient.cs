using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;

namespace IAX.Client
{
    public class IAXClient : Component
    {
        public event LevelsEventHandler LevelsEvent;
        public event NetStatsEventHandler NetStatsEvent;
        public event RegistrationEventHandler RegistrationEvent;
        public event StateEventHandler StateEvent;
        public event TextEventHandler TextEvent;
        public event UrlEventHandler UrlEvent;
        public event VideoEventHandler VideoEvent;

        private IAXUnmanaged.iaxc_event_callback_t CallbackDelegate;

        public IAXClient() : this(IAXAudioType.Internal, 1)
        {
        }

        public IAXClient(IAXAudioType AudioType) : this(AudioType, 1)
        {
        }

        public IAXClient(int NumCalls) : this(IAXAudioType.Internal, NumCalls)
        {
        }

        public IAXClient(IAXAudioType AudioType, int NumCalls)
        {
            IAXUnmanaged.iaxc_initialize((int)AudioType, NumCalls);
            CallbackDelegate = new IAXUnmanaged.iaxc_event_callback_t(IAXCallback);
            IAXUnmanaged.iaxc_set_event_callback(CallbackDelegate);
            IAXUnmanaged.iaxc_start_processing_thread();
        }

        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);
            IAXUnmanaged.iaxc_stop_processing_thread();
            IAXUnmanaged.iaxc_shutdown();
        }

        public IAXFilter Filters
        {
            get
            {
                return IAXUnmanaged.iaxc_get_filters();
            }
            set
            {
                IAXUnmanaged.iaxc_set_filters(value);
            }
        }

        public bool MicBoost
        {
            get
            {
                return IAXUnmanaged.iaxc_mic_boost_get() != 0;
            }
            set
            {
                IAXUnmanaged.iaxc_mic_boost_set(value ? 1 : 0);
            }
        }

        internal unsafe int IAXCallback(IAXUnmanaged.iaxc_event e)
        {
            IAXUnmanaged.iaxc_event ev = e;
            while (true)
            {
                switch (ev.type)
                {
                    case IAXUnmanaged.IAXC_EVENT_LEVELS:
                        if (LevelsEvent != null)
                            LevelsEvent(this, new IAXLevelsEventArgs(ev));
                        break;
                    case IAXUnmanaged.IAXC_EVENT_NETSTAT:
                        if (NetStatsEvent != null)
                            NetStatsEvent(this, new IAXNetStatsEventArgs(ev));
                        break;
                    case IAXUnmanaged.IAXC_EVENT_REGISTRATION:
                        if (RegistrationEvent != null)
                            RegistrationEvent(this, new IAXRegistrationEventArgs(ev));
                        break;
                    case IAXUnmanaged.IAXC_EVENT_STATE:
                        if (StateEvent != null)
                            StateEvent(this, new IAXStateEventArgs(ev));
                        break;
                    case IAXUnmanaged.IAXC_EVENT_TEXT:
                        if (TextEvent != null)
                            TextEvent(this, new IAXTextEventArgs(ev));
                        break;
                    case IAXUnmanaged.IAXC_EVENT_URL:
                        if (UrlEvent != null)
                            UrlEvent(this, new IAXUrlEventArgs(ev));
                        break;
                    case IAXUnmanaged.IAXC_EVENT_VIDEO:
                        if (VideoEvent != null)
                            VideoEvent(this, new IAXVideoEventArgs(ev));
                        break;
                }

                // see if there is a "next" event
                if (ev.next == null)
                    return 0;
                else
                    ev = *ev.next;
            }
        }

        public int Register(string User, string Password, string Server)
        {
            return IAXUnmanaged.iaxc_register(User, Password, Server);
        }

        public int Unregister(int ID)
        {
            return IAXUnmanaged.iaxc_unregister(ID);
        }

        public void Call(string IAX)
        {
            IAXUnmanaged.iaxc_call(IAX);
        }

        public void Call(string Server, string Number)
        {
            Call(String.Format("{0}/{1}", Server, Number));
        }

        public void Call(string User, string Server, string Number)
        {
            Call(String.Format("{0}@{1}/{2}", User, Server, Number));
        }

        public void Call(string User, string Password, string Server, string Number)
        {
            Call(String.Format("{0}:{1}@{2}/{3}", User, Password, Server, Number));
        }

        public void Transfer(string number)
        {
            IAXUnmanaged.iaxc_blind_transfer_call(IAXUnmanaged.iaxc_selected_call(), number);
        }

        public void Hangup()
        {
            IAXUnmanaged.iaxc_dump_call();
        }

        public void SetFormats(IAXFormat preferred, IAXFormat allowed)
        {
            IAXUnmanaged.iaxc_set_formats((int)preferred, (int)allowed);
        }

        public void Answer()
        {
            IAXUnmanaged.iaxc_answer_call(0);
        }


   }
}

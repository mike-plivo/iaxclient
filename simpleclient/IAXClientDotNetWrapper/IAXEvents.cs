using System;
using System.Collections.Generic;
using System.Text;

namespace IAX.Client
{
    public abstract class IAXEventArgs : EventArgs
    {
        internal IAXEventArgs(IAXUnmanaged.iaxc_event ev)
        {
        }

        protected static unsafe string BytesToString(byte* bs)
        {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < IAXUnmanaged.IAXC_EVENT_BUFSIZ; i++)
                if (bs[i] == 0)
                    return sb.ToString();
                else
                    sb.Append((char)bs[i]);
            return sb.ToString();
        }
    }

    public class IAXLevelsEventArgs : IAXEventArgs
    {
        public float Input;
        public float Output;

        internal IAXLevelsEventArgs(IAXUnmanaged.iaxc_event e)
            : base(e)
        {
            Input = e.levels.input;
            Output = e.levels.output;
        }
    }

    public class IAXNetStats
    {
        public int Jitter;
        public int LossPercent;
        public int LossCount;
        public int Packets;
        public int Delay;
        public int Dropped;
        public int OutOfOrder;

        internal IAXNetStats(IAXUnmanaged.iaxc_netstat n)
        {
            Jitter = n.jitter;
            LossPercent = n.losspct;
            LossCount = n.losscnt;
            Packets = n.packets;
            Delay = n.delay;
            Dropped = n.dropped;
            OutOfOrder = n.ooo;
        }
    }

    public class IAXNetStatsEventArgs : IAXEventArgs
    {
        public int CallNo;
        public int RoundTripTime;
        public IAXNetStats Local;
        public IAXNetStats Remote;

        internal unsafe IAXNetStatsEventArgs(IAXUnmanaged.iaxc_event e)
            : base(e)
        {
            CallNo = e.netstats.callNo;
            RoundTripTime = e.netstats.rtt;
            Local = new IAXNetStats(e.netstats.local);
            Remote = new IAXNetStats(e.netstats.remote);
        }

        public override string ToString()
        {
            string f = "J={0} L={2} {1}% P={3} D={4} Dr={5} O={6}";
            string l = String.Format(f, Local.Jitter, Local.LossPercent, Local.LossCount, Local.Packets, Local.Delay, Local.Dropped, Local.OutOfOrder);
            string r = String.Format(f, Remote.Jitter, Remote.LossPercent, Remote.LossCount, Remote.Packets, Remote.Delay, Remote.Dropped, Remote.OutOfOrder);
            return String.Format("RTT={0} Local[{1}] Remote[{2}]", RoundTripTime, l, r);
        }
    }

    public class IAXRegistrationEventArgs : IAXEventArgs
    {
        public int ID;
        public IAXRegistrationReply Reply;
        public int MessageCount;

        internal unsafe IAXRegistrationEventArgs(IAXUnmanaged.iaxc_event e)
            : base(e)
        {
            ID = e.reg.id;
            Reply = (IAXRegistrationReply)e.reg.reply;
            MessageCount = e.reg.msgcount;
        }
    }

    public class IAXStateEventArgs : IAXEventArgs
    {
        public int CallNo;
        public IAXCallState State;
        public IAXFormat Format;
        public string Remote;
        public string RemoteName;
        public string Local;
        public string LocalContext;

        internal unsafe IAXStateEventArgs(IAXUnmanaged.iaxc_event e)
            : base(e)
        {
            CallNo = e.call.callNo;
            State = (IAXCallState)e.call.state;
            Format = (IAXFormat)e.call.format;
            Remote = BytesToString(e.call.remote);
            RemoteName = BytesToString(e.call.remote_name);
            Local = BytesToString(e.call.local);
            LocalContext = BytesToString(e.call.local_context);
        }
    }

    public class IAXTextEventArgs : IAXEventArgs
    {
        public IAXTTextEventType Type;
        public int CallNo;
        public string Message;

        internal unsafe IAXTextEventArgs(IAXUnmanaged.iaxc_event e)
            : base(e)
        {
            Type = (IAXTTextEventType)e.text.type;
            CallNo = e.text.callNo;
            Message = BytesToString(e.text.message);
        }
    }

    public class IAXUrlEventArgs : IAXEventArgs
    {
        public int CallNo;
        public IAXUrlEventType Type;
        public string Url;

        internal unsafe IAXUrlEventArgs(IAXUnmanaged.iaxc_event e)
            : base(e)
        {
            CallNo = e.url.callNo;
            Type = (IAXUrlEventType)e.url.type;
            Url = BytesToString(e.url.url);
        }
    }

    public class IAXVideoEventArgs : IAXEventArgs
    {
        public int CallNo;
        public int Format; // TODO: should be enum?
        public int Width;
        public int Height;
        public object Data;

        internal unsafe IAXVideoEventArgs(IAXUnmanaged.iaxc_event e)
            : base(e)
        {
            CallNo = e.video.callNo;
            Format = e.video.format;
            Width = e.video.width;
            Height = e.video.height;
            Data = null; // TODO: how to interpret data?
        }
    }

    public delegate void LevelsEventHandler(object sender, IAXLevelsEventArgs e);
    public delegate void NetStatsEventHandler(object sender, IAXNetStatsEventArgs e);
    public delegate void RegistrationEventHandler(object sender, IAXRegistrationEventArgs e);
    public delegate void StateEventHandler(object sender, IAXStateEventArgs e);
    public delegate void TextEventHandler(object sender, IAXTextEventArgs e);
    public delegate void UrlEventHandler(object sender, IAXUrlEventArgs e);
    public delegate void VideoEventHandler(object sender, IAXVideoEventArgs e);
}

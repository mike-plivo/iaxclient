This VS 2005 project builds a managed wrapper for iaxclient.  It copies the DLL 
from the vs2005 directory using a VS build event.  This is referenced by the lines 
like these in IAXUnmanaged.cs:

        [DllImport("iaxclient.dll", CallingConvention = CallingConvention.StdCall)]

Create an IAXClient, register for events and use the methods to make calls etc:

            IAXClient iax = new IAXClient();
            iax.SetFormats(IAXFormat.ULAW, IAXFormat.ULAW | IAXFormat.ALAW);
            StateDelegate = new StateEventHandler(SafeStateEvent);
            iax.StateEvent += new StateEventHandler(iax_StateEvent);

            iax.Call(user, password, server, number);
...
            iax.Hangup();

The events are handled via BeginInvoke because they may be raised on a different 
thread to the UI (see .NET documentation about this):

        private void iax_StateEvent(object sender, IAXStateEventArgs e)
        {
            if (!labelCallStatus.IsDisposed) // labelCallStatus is some control in the UI
                labelCallStatus.BeginInvoke(StateDelegate, new object[] { sender, e });
        }

        private void SafeStateEvent(object sender, IAXStateEventArgs e)
        {
            if (labelCallStatus.IsDisposed)
                return;
            labelCallStatus.Text = e.State.ToString();
        }

Plenty of iaxclient fucntionality is not covered, but it should be straightforward
to extend it.

David Brazier
david@brazier.me.uk
June 2007
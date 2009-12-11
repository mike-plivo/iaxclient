#
# Tcl package index file
#
package ifneeded iaxclient 0.2 \
    "[list load [file join $dir tcliaxclient0.2.dylib] tcliaxclient]; \
    [list source [file join $dir iaxclient.tcl]]"


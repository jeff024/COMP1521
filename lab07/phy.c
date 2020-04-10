physicalAddress (vAddr, action):
    extract page# and offset from vAddr
    if the page# is not valid, return -1

    if the page is already loaded:
        set the Modified flag if action is a write,
        update the access time to the current clock tick, and
        use the frame number and offset to compute a physical address

    otherwise:
        look for an unused frame;
        if we find one, use that,
        otherwise:
            // we need to replace a currently loaded frame, so
            find the Least Recently Used loaded page,
            set its PageTable entry to indicate "no longer loaded",
            increment the nReplaces counter,
            increment the nSaves counter if modified, and
            use the frame that backed that page.

        // we should now have a frame# to use, so:
        increment the nLoads counter,
        set PageTable entry for the new page
            (flags, frame#, accesstime=current clock tick), and
        use the frame number and offset to compute a physical address

    return the physical address
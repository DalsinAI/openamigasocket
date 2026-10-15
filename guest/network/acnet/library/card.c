/* OpenSocket's card layer: the backend is chosen once, at the library's
 * init. That is its first OpenLibrary, which ramlib makes after DOS, in
 * ramlib's own process, so the configuration can be read from disk. The
 * order (OpenSocket's own configuration first, then the card, then a
 * Roadshow interface file):
 *
 *  1. ENV:OpenSocket/Interface (or ENVARC:OpenSocket/Interface) names a
 *     backend, or a device: that backend.
 *  2. The OpenSocket card is fitted (Zorro II, Dalsin, product 6): Host, through
 *     opensocket.library and DEVS:Networks/opensocket.device, both from disk.
 *  3. A Roadshow interface file in DEVS:NetInterfaces/ names a SANA-II
 *     device: Direct, OpenSocket's own stack over that device.
 *  4. None of these: no backend. The init keeps nothing, so ramlib goes on
 *     to LIBS:bsdsocket.library (another stack), as for any ROM library
 *     whose init kept nothing.
 *
 * The files are keyword=value lines ("keyword value" is taken too), with
 * '#' or ';' starting a comment. OpenSocket's own keywords: backend
 * (host, direct or none), device, unit, configure (dhcp), address, netmask,
 * gateway and nameserver (up to three). From a Roadshow interface file only
 * its documented keywords are read: device, unit, configure=dhcp, address
 * and netmask; the rest is skipped. Name servers for Direct come from
 * DEVS:Internet/name_resolution's "nameserver" lines when the OpenSocket
 * file names none. No requester is shown for a missing ENV: or DEVS:.
 *
 * Nothing here is kept in a global: the choice goes into the master
 * base's shared block (acnet_lib.h), so the library runs from a ROM. MIT. */
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <dos/exall.h>
#include <libraries/configvars.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/expansion.h>

#include "acnet_lib.h"
#include "achostsocket.h"
#include "card.h"

#define WORK_FILE 2048   /* the most of one file that is read */
#define WORK_PATH 160

struct Work {
    char path[WORK_PATH];
    char buf[WORK_FILE + 1];
};

/* A file into w->buf: its length, or -1 when it can't be read. */
static LONG read_file(struct DosLibrary *DOSBase, struct Work *w, const char *name)
{
    BPTR fh = Open((STRPTR)name, MODE_OLDFILE);
    LONG n;
    if (!fh) return -1;
    n = Read(fh, w->buf, WORK_FILE);
    Close(fh);
    if (n < 0) return -1;
    w->buf[n] = 0;
    return n;
}

static LONG parse_file(struct DosLibrary *DOSBase, struct Work *w, struct CardConfig *cfg,
                       const char *name, int ours)
{
    LONG n = read_file(DOSBase, w, name);
    return n < 0 ? -1 : card_parse(cfg, w->buf, n, ours);
}

/* The first file in DEVS:NetInterfaces/ that names a device (not icons,
 * not hidden names): 1, with cfg filled from it. */
static int roadshow_interface(struct DosLibrary *DOSBase, struct Work *w, struct CardConfig *cfg)
{
    static const char dir[] = "DEVS:NetInterfaces";
    struct FileInfoBlock *fib;
    BPTR lock = Lock((STRPTR)dir, SHARED_LOCK);
    int found = 0;
    if (!lock) return 0;
    fib = AllocDosObject(DOS_FIB, NULL);
    if (fib && Examine(lock, fib)) {
        while (!found && ExNext(lock, fib)) {
            const char *nm = (const char *)fib->fib_FileName;
            LONG len = 0, i;
            struct CardConfig one;
            while (nm[len]) len++;
            if (fib->fib_DirEntryType >= 0 || !len || nm[0] == '.' || nm[0] == '~') continue;
            if (len >= 5 && card_key_is(nm + len - 5, 5, ".info")) continue;
            if (len + (LONG)sizeof(dir) + 1 > WORK_PATH) continue;
            for (i = 0; dir[i]; i++) w->path[i] = dir[i];
            w->path[i++] = '/';
            card_copy(w->path + i, WORK_PATH - i, nm, len);
            one = *cfg;
            if (parse_file(DOSBase, w, &one, w->path, 0) > 0 && one.device[0]) {
                *cfg = one;
                card_copy(cfg->ifname, sizeof(cfg->ifname), nm, len);
                found = 1;
            }
        }
    }
    if (fib) FreeDosObject(DOS_FIB, fib);
    UnLock(lock);
    return found;
}

/* The OpenSocket card (Zorro II, Dalsin or the old 2011, product 6). */
static int card_fitted(void)
{
    struct Library *ExpansionBase = OpenLibrary((CONST_STRPTR)"expansion.library", 0);
    struct ConfigDev *cd;
    if (!ExpansionBase) return 0;
    cd = FindConfigDev(NULL, ACNET_MANUFACTURER, ACNET_PRODUCT);
    if (!cd) cd = FindConfigDev(NULL, ACNET_MANUFACTURER_OLD, ACNET_PRODUCT);
    CloseLibrary(ExpansionBase);
    return cd != NULL;
}

LONG card_choose(struct CardConfig *cfg)
{
    static const char ifname[] = "opensocket0";
    struct DosLibrary *DOSBase = NULL;
    struct Process *me = (struct Process *)FindTask(NULL);
    struct Work *w = NULL;
    APTR oldwin = NULL;
    LONG i;
    int files = 0;

    for (i = 0; i < (LONG)sizeof(*cfg); i++) ((UBYTE *)cfg)[i] = 0;
    card_copy(cfg->ifname, sizeof(cfg->ifname), ifname, sizeof(ifname) - 1);

    /* Files only from a process (ramlib is one), and with no requester for
     * a volume or assign that isn't there. */
    if (me->pr_Task.tc_Node.ln_Type == NT_PROCESS) {
        DOSBase = (struct DosLibrary *)OpenLibrary((CONST_STRPTR)"dos.library", 37);
        w = AllocMem(sizeof(struct Work), MEMF_ANY);
        if (DOSBase && w) {
            files = 1;
            oldwin = me->pr_WindowPtr;
            me->pr_WindowPtr = (APTR)-1;
        }
    }

    if (files) {
        LONG got = parse_file(DOSBase, w, cfg, "ENV:OpenSocket/Interface", 1);
        if (got < 0) got = parse_file(DOSBase, w, cfg, "ENVARC:OpenSocket/Interface", 1);
        if (got > 0) {
            if (!(got & GOT_BACKEND)) cfg->backend = BACKEND_DIRECT;   /* a device alone */
            cfg->chosen_by = CHOSEN_BY_CONFIG;
            goto done;
        }
    }
    if (card_fitted()) {
        cfg->backend = BACKEND_HOST;
        cfg->chosen_by = CHOSEN_BY_CARD;
        goto done;
    }
    if (files && roadshow_interface(DOSBase, w, cfg)) {
        cfg->backend = BACKEND_DIRECT;
        cfg->chosen_by = CHOSEN_BY_ROADSHOW;
    }
done:
    if (files && cfg->backend == BACKEND_DIRECT && !cfg->nameserver[0])
        parse_file(DOSBase, w, cfg, "DEVS:Internet/name_resolution", 0);
    if (!cfg->address && !cfg->dhcp) cfg->dhcp = 1;   /* no address set: DHCP */
    if (files) me->pr_WindowPtr = oldwin;
    if (w) FreeMem(w, sizeof(struct Work));
    if (DOSBase) CloseLibrary((struct Library *)DOSBase);
    return cfg->backend;
}

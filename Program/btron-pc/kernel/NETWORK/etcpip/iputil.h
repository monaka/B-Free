#ifndef __IPUTIL_H__
#define __IPUTIL_H__

/* byte conversion utility */
export UW htonl(UW hl); /* Convert Host byte order to network byte order (32bits) */
export UH htons(UH hs); /* Convert host byte order to network byte order (16bits) */
export UW ntohl(UW nl); /* byte order network to host (32bits) */
export UH ntohs(UH ns); /* byte order network to host (16bits) */

/* Error report routine */
export ER mainercd(ER ercd); /* Get main error code from error code */
export ER sbuercd(ER ercd);  /* Get sub error code from error code */

/* Utility macros */
#define ID_CHECK(repid) { if (repid <= 1) { return E_ID; } }
#define TMO_CHECK(tmo) { if (tmo < -2) { return E_PAR; } }
#define NUL_CHECK(v) { if ((v) == NULL) { return E_PAR; } }
#define LEN_CHECK(v) { if ((v) < 0) { return E_PAR; } }

#endif


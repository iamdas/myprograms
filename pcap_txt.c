PCAP(3)                                                                PCAP(3)



NAME
       pcap - Packet Capture library

SYNOPSIS
       #include <pcap.h>

       char errbuf[PCAP_ERRBUF_SIZE];

       pcap_t *pcap_open_live(const char *device, int snaplen,
               int promisc, int to_ms, char *errbuf)
       pcap_t *pcap_open_dead(int linktype, int snaplen)
       pcap_t *pcap_open_offline(const char *fname, char *errbuf)
       pcap_t *pcap_fopen_offline(FILE *fp, char *errbuf)
       pcap_dumper_t *pcap_dump_open(pcap_t *p, const char *fname)
       pcap_dumper_t *pcap_dump_fopen(pcap_t *p, FILE *fp)

       int pcap_setnonblock(pcap_t *p, int nonblock, char *errbuf);
       int pcap_getnonblock(pcap_t *p, char *errbuf);

       int pcap_findalldevs(pcap_if_t **alldevsp, char *errbuf)
       void pcap_freealldevs(pcap_if_t *alldevs)
       char *pcap_lookupdev(char *errbuf)
       int pcap_lookupnet(const char *device, bpf_u_int32 *netp,
               bpf_u_int32 *maskp, char *errbuf)

       typedef void (*pcap_handler)(u_char *user, const struct pcap_pkthdr *h,
                                   const u_char *bytes);
       int pcap_dispatch(pcap_t *p, int cnt,
               pcap_handler callback, u_char *user)
       int pcap_loop(pcap_t *p, int cnt,
               pcap_handler callback, u_char *user)
       void pcap_dump(u_char *user, struct pcap_pkthdr *h,
               u_char *sp)

       int pcap_compile(pcap_t *p, struct bpf_program *fp,
               const char *str, int optimize, bpf_u_int32 netmask)
       int pcap_setfilter(pcap_t *p, struct bpf_program *fp)
       void pcap_freecode(struct bpf_program *)
       int pcap_setdirection(pcap_t *p, pcap_direction_t d)

       const u_char *pcap_next(pcap_t *p, struct pcap_pkthdr *h)
       int pcap_next_ex(pcap_t *p, struct pcap_pkthdr **pkt_header,
               const u_char **pkt_data)

       void pcap_breakloop(pcap_t *)

       int pcap_inject(pcap_t *p, const void *buf, size_t size)
       int pcap_sendpacket(pcap_t *p, const u_char *buf, int size)

       int pcap_datalink(pcap_t *p)
       int pcap_list_datalinks(pcap_t *p, int **dlt_buf);
       int pcap_set_datalink(pcap_t *p, int dlt);
       int pcap_datalink_name_to_val(const char *name);
       const char *pcap_datalink_val_to_name(int dlt);
       const char *pcap_datalink_val_to_description(int dlt);
       int pcap_snapshot(pcap_t *p)
       int pcap_is_swapped(pcap_t *p)
       int pcap_major_version(pcap_t *p)
       int pcap_minor_version(pcap_t *p)
       int pcap_stats(pcap_t *p, struct pcap_stat *ps)
       FILE *pcap_file(pcap_t *p)
       int pcap_fileno(pcap_t *p)
       int pcap_get_selectable_fd(pcap_t *p);
       void pcap_perror(pcap_t *p, char *prefix)
       char *pcap_geterr(pcap_t *p)
       const char *pcap_strerror(int error)
       const char *pcap_lib_version(void)

       void pcap_close(pcap_t *p)
       int pcap_dump_flush(pcap_dumper_t *p)
       long pcap_dump_ftell(pcap_dumper_t *p)
       FILE *pcap_dump_file(pcap_dumper_t *p)
       void pcap_dump_close(pcap_dumper_t *p)

DESCRIPTION
       The  Packet  Capture  library provides a high level interface to packet
       capture systems. All packets on the network, even  those  destined  for
       other hosts, are accessible through this mechanism.


ROUTINES
       NOTE:      errbuf      in      pcap_open_live(),      pcap_open_dead(),
       pcap_open_offline(),     pcap_fopen_offline(),      pcap_setnonblock(),
       pcap_getnonblock(),     pcap_findalldevs(),    pcap_lookupdev(),    and
       pcap_lookupnet()  is  assumed   to   be   able   to   hold   at   least
       PCAP_ERRBUF_SIZE chars.

       pcap_open_live()  is used to obtain a packet capture descriptor to look
       at packets on the network.  device is a string that specifies the  net-
       work  device  to  open;  on  Linux systems with 2.2 or later kernels, a
       device argument of "any" or NULL can be used to  capture  packets  from
       all  interfaces.  snaplen specifies the maximum number of bytes to cap-
       ture.  If this value is less than the size of a  packet  that  is  cap-
       tured, only the first snaplen bytes of that packet will be captured and
       provided as packet data.  A value of 65535  should  be  sufficient,  on
       most  if  not  all networks, to capture all the data available from the
       packet.  promisc specifies if the interface is to be put into promiscu-
       ous  mode.   (Note  that even if this parameter is false, the interface
       could well be in promiscuous mode for some  other  reason.)   For  now,
       this  doesn't work on the "any" device; if an argument of "any" or NULL
       is supplied, the promisc flag is ignored.   to_ms  specifies  the  read
       timeout  in milliseconds.  The read timeout is used to arrange that the
       read not necessarily return immediately when a packet is seen, but that
       it  wait for some amount of time to allow more packets to arrive and to
       read multiple packets from the OS kernel in  one  operation.   Not  all
       platforms  support  a  read  timeout; on platforms that don't, the read
       timeout is ignored.  A zero value for to_ms, on platforms that  support
       a read timeout, will cause a read to wait forever to allow enough pack-
       ets to arrive, with no timeout.  errbuf is  used  to  return  error  or
       warning text.  It will be set to error text when pcap_open_live() fails
       and returns NULL.   errbuf  may  also  be  set  to  warning  text  when
       pcap_open_live() succeds; to detect this case the caller should store a
       zero-length string in errbuf before calling pcap_open_live()  and  dis-
       play  the  warning  to  the  user  if errbuf is no longer a zero-length
       string.

       pcap_open_dead() is used for creating a pcap_t structure  to  use  when
       calling the other functions in libpcap.  It is typically used when just
       using libpcap for compiling BPF code.

       pcap_open_offline() is called  to  open  a  ``savefile''  for  reading.
       fname  specifies  the  name  of the file to open. The file has the same
       format as those used by tcpdump(8) and tcpslice(1).  The name "-" in  a
       synonym for stdin.  Alternatively, you may call pcap_fopen_offline() to
       read dumped data from an existing open stream fp.  Note  that  on  Win-
       dows,  that  stream should be opened in binary mode.  errbuf is used to
       return  error  text  and  is  only  set  when  pcap_open_offline()   or
       pcap_fopen_offline() fails and returns NULL.

       pcap_dump_open() is called to open a ``savefile'' for writing. The name
       "-" in a synonym for stdout.  NULL is returned on failure.  p is a pcap
       struct  as  returned by pcap_open_offline() or pcap_open_live().  fname
       specifies the name of the file to open.  Alternatively,  you  may  call
       pcap_dump_fopen()  to  write  data to an existing open stream fp.  Note
       that on Windows, that stream should be opened in binary mode.  If  NULL
       is returned, pcap_geterr() can be used to get the error text.


       pcap_setnonblock()    puts    a   capture   descriptor,   opened   with
       pcap_open_live(), into ``non-blocking'' mode, or takes it out of ``non-
       blocking'' mode, depending on whether the nonblock argument is non-zero
       or zero.  It has no effect on ``savefiles''.  If there is an error,  -1
       is  returned and errbuf is filled in with an appropriate error message;
       otherwise, 0 is returned.  In ``non-blocking'' mode, an attempt to read
       from  the  capture  descriptor with pcap_dispatch() will, if no packets
       are currently available to be read, return 0  immediately  rather  than
       blocking  waiting  for  packets to arrive.  pcap_loop() and pcap_next()
       will not work in ``non-blocking'' mode.

       pcap_getnonblock() returns the current ``non-blocking''  state  of  the
       capture  descriptor; it always returns 0 on ``savefiles''.  If there is
       an error, -1 is returned and errbuf is filled in  with  an  appropriate
       error message.

       pcap_findalldevs()  constructs  a  list  of network devices that can be
       opened with pcap_open_live().  (Note that there may be network  devices
       that  cannot  be  opened  with  pcap_open_live() by the process calling
       pcap_findalldevs(), because, for example, that process might  not  have
       sufficient  privileges to open them for capturing; if so, those devices
       will not appear on the list.)  alldevsp is set to point  to  the  first
       element of the list; each element of the list is of type pcap_if_t, and
       has the following members:

              next   if not NULL, a pointer to the next element in  the  list;
                     NULL for the last element of the list

              name   a  pointer  to  a  string giving a name for the device to
                     pass to pcap_open_live()

              description
                     if not NULL, a pointer to a string giving  a  human-read-
                     able description of the device

              addresses
                     a pointer to the first element of a list of addresses for
                     the interface

              flags  interface flags:

                     PCAP_IF_LOOPBACK
                             set if the interface is a loopback interface

       Each element of the list of addresses is of type pcap_addr_t,  and  has
       the following members:

              next   if  not  NULL, a pointer to the next element in the list;
                     NULL for the last element of the list

              addr   a pointer to a struct sockaddr containing an address

              netmask
                     if not NULL, a pointer to a struct sockaddr that contains
                     the  netmask  corresponding  to the address pointed to by
                     addr

              broadaddr
                     if not NULL, a pointer to a struct sockaddr that contains
                     the   broadcast  address  corresponding  to  the  address
                     pointed to by addr; may be null if the interface  doesn't
                     support broadcasts

              dstaddr
                     if not NULL, a pointer to a struct sockaddr that contains
                     the destination  address  corresponding  to  the  address
                     pointed  to by addr; may be null if the interface isn't a
                     point-to-point interface

       Note that not all the addresses in the list of addresses are  necessar-
       ily IPv4 or IPv6 addresses - you must check the sa_family member of the
       struct sockaddr before interpreting the contents of the address.

       -1 is returned on failure, in which case errbuf is filled  in  with  an
       appropriate error message; 0 is returned on success.

       pcap_freealldevs()  is  used  to  free  a  list allocated by pcap_find-
       alldevs().

       pcap_lookupdev() returns a pointer to a network device suitable for use
       with pcap_open_live() and pcap_lookupnet().  If there is an error, NULL
       is returned and errbuf is filled in with an appropriate error  message.

       pcap_lookupnet() is used to determine the network number and mask asso-
       ciated with the  network  device  device.   Both  netp  and  maskp  are
       bpf_u_int32  pointers.  A return of -1 indicates an error in which case
       errbuf is filled in with an appropriate error message.

       pcap_dispatch() is used to collect and process packets.  cnt  specifies
       the maximum number of packets to process before returning.  This is not
       a minimum number; when reading a live capture, only  one  bufferful  of
       packets  is read at a time, so fewer than cnt packets may be processed.
       A cnt of -1 processes all the packets received in one buffer when read-
       ing  a  live  capture,  or  all  the packets in the file when reading a
       ``savefile''.  callback specifies a pcap_handler routine to  be  called
       with  three  arguments:  a  u_char  pointer  which  is  passed  in from
       pcap_dispatch(), a const struct pcap_pkthdr pointer to a structure with
       the following members:

              ts     a  struct timeval containing the time when the packet was
                     captured

              caplen a bpf_u_int32 giving the number of bytes  of  the  packet
                     that are available from the capture

              len    a  bpf_u_int32  giving the length of the packet, in bytes
                     (which might be more than the number of  bytes  available
                     from  the  capture, if the length of the packet is larger
                     than the maximum number of bytes to capture)

       and a const u_char pointer to the first caplen (as given in the  struct
       pcap_pkthdr a pointer to which is passed to the callback routine) bytes
       of data from the packet (which won't necessarily be the entire  packet;
       to  capture  the  entire  packet,  you will have to provide a value for
       snaplen in your call to pcap_open_live() that is sufficiently large  to
       get all of the packet's data - a value of 65535 should be sufficient on
       most if not all networks).

       The number of packets read is returned.  0 is returned  if  no  packets
       were  read  from  a  live capture (if, for example, they were discarded
       because they didn't pass the packet filter, or if,  on  platforms  that
       support  a  read  timeout  that  starts  before any packets arrive, the
       timeout expires before any packets arrive, or if  the  file  descriptor
       for  the  capture  device  is  in non-blocking mode and no packets were
       available to be read) or if no more packets are available in a  ``save-
       file.''  A  return of -1 indicates an error in which case pcap_perror()
       or pcap_geterr() may be used to display the error text.  A return of -2
       indicates  that  the  loop terminated due to a call to pcap_breakloop()
       before  any  packets  were  processed.   If   your   application   uses
       pcap_breakloop(),  make  sure  that you explicitly check for -1 and -2,
       rather than just checking for a return value < 0.

       NOTE: when reading a live capture, pcap_dispatch() will not necessarily
       return  when  the  read  times out; on some platforms, the read timeout
       isn't supported, and, on other platforms, the timer doesn't start until
       at  least  one packet arrives.  This means that the read timeout should
       NOT be used in, for example, an interactive application, to  allow  the
       packet capture loop to ``poll'' for user input periodically, as there's
       no  guarantee  that  pcap_dispatch()  will  return  after  the  timeout
       expires.

       pcap_loop() is similar to pcap_dispatch() except it keeps reading pack-
       ets until cnt packets are processed or an error occurs.   It  does  not
       return  when  live  read timeouts occur.  Rather, specifying a non-zero
       read timeout  to  pcap_open_live()  and  then  calling  pcap_dispatch()
       allows the reception and processing of any packets that arrive when the
       timeout occurs.  A negative cnt causes pcap_loop() to loop forever  (or
       at  least  until  an  error  occurs).  -1 is returned on an error; 0 is
       returned if cnt is exhausted; -2 is returned if the loop terminated due
       to  a  call  to pcap_breakloop() before any packets were processed.  If
       your application uses pcap_breakloop(), make sure that  you  explicitly
       check  for -1 and -2, rather than just checking for a return value < 0.

       pcap_next() reads the next packet (by calling  pcap_dispatch()  with  a
       cnt  of  1)  and  returns  a u_char pointer to the data in that packet.
       (The pcap_pkthdr struct for that packet  is  not  supplied.)   NULL  is
       returned  if  an  error occured, or if no packets were read from a live
       capture (if, for example, they were discarded because they didn't  pass
       the packet filter, or if, on platforms that support a read timeout that
       starts before any packets arrive, the timeout expires before any  pack-
       ets arrive, or if the file descriptor for the capture device is in non-
       blocking mode and no packets were available to be read), or if no  more
       packets  are  available  in a ``savefile.''  Unfortunately, there is no
       way to determine whether an error occured or not.

       pcap_next_ex() reads the next  packet  and  returns  a  success/failure
       indication:

              1      the packet was read without problems

              0      packets are being read from a live capture, and the time-
                     out expired

              -1     an error occurred while reading the packet

              -2     packets are being read from a ``savefile'', and there are
                     no more packets to read from the savefile.

       If  the packet was read without problems, the pointer pointed to by the
       pkt_header argument is set to point to the pcap_pkthdr struct  for  the
       packet,  and  the pointer pointed to by the pkt_data argument is set to
       point to the data in the packet.

       pcap_breakloop()  sets  a  flag  that  will  force  pcap_dispatch()  or
       pcap_loop()  to return rather than looping; they will return the number
       of packets that have been processed so far, or -2 if  no  packets  have
       been processed so far.

       This  routine  is safe to use inside a signal handler on UNIX or a con-
       sole control handler on Windows, as it  merely  sets  a  flag  that  is
       checked within the loop.

       The  flag is checked in loops reading packets from the OS - a signal by
       itself will not necessarily terminate those loops - as well as in loops
       processing  a  set of packets returned by the OS.  Note that if you are
       catching signals on UNIX systems that support restarting  system  calls
       after a signal, and calling pcap_breakloop() in the signal handler, you
       must specify, when catching those signals, that system calls should NOT
       be  restarted  by  that signal.  Otherwise, if the signal interrupted a
       call reading packets in  a  live  capture,  when  your  signal  handler
       returns after calling pcap_breakloop(), the call will be restarted, and
       the loop will not terminate until more packets arrive and the call com-
       pletes.

       Note  also  that,  in  a  multi-threaded  application, if one thread is
       blocked    in    pcap_dispatch(),    pcap_loop(),    pcap_next(),    or
       pcap_next_ex(),  a  call to pcap_breakloop() in a different thread will
       not unblock that thread; you will need to use whatever mechanism the OS
       provides  for  breaking  a  thread  out  of  blocking calls in order to
       unblock the thread, such as thread cancellation in systems that support
       POSIX threads.

       Note  that  pcap_next()  will,  on some platforms, loop reading packets
       from the OS; that loop will not necessarily be terminated by a  signal,
       so  pcap_breakloop() should be used to terminate packet processing even
       if pcap_next() is being used.

       pcap_breakloop() does not guarantee that no  further  packets  will  be
       processed by pcap_dispatch() or pcap_loop() after it is called; at most
       one more packet might be processed.

       If -2 is returned from pcap_dispatch()  or  pcap_loop(),  the  flag  is
       cleared,  so a subsequent call will resume reading packets.  If a posi-
       tive number is returned, the flag is not cleared, so a subsequent  call
       will return -2 and clear the flag.

       pcap_inject()  sends  a  raw  packet through the network interface; buf
       points to the data of the packet, including the link-layer header,  and
       size  is  the  number of bytes in the packet.  It returns the number of
       bytes written on success.  A return of -1 indicates an error  in  which
       case  pcap_perror()  or  pcap_geterr() may be used to display the error
       text.  Note that, even if you successfully open the network  interface,
       you  might  not  have permission to send packets on it, or it might not
       support sending packets; as pcap_open_live() doesn't  have  a  flag  to
       indicate whether to open for capturing, sending, or capturing and send-
       ing, you cannot request an open that supports sending and  be  notified
       at  open  time  whether  sending will be possible.  Note also that some
       devices might not support sending packets.

       Note that, on some platforms,  the  link-layer  header  of  the  packet
       that's  sent  might  not  be  the  same as the link-layer header of the
       packet supplied to pcap_inject(), as the source link-layer address,  if
       the header contains such an address, might be changed to be the address
       assigned to the interface on which the packet it sent, if the  platform
       doesn't  support  sending  completely  raw and unchanged packets.  Even
       worse, some drivers on some platforms might change the link-layer  type
       field to whatever value libpcap used when attaching to the device, even
       on platforms that do  nominally  support  sending  completely  raw  and
       unchanged packets.

       pcap_sendpacket()  is  like  pcap_inject(), but it returns 0 on success
       and -1 on  failure.   (pcap_inject()  comes  from  OpenBSD;  pcap_send-
       packet() comes from WinPcap.  Both are provided for compatibility.)

       pcap_dump()   outputs   a   packet  to  the  ``savefile''  opened  with
       pcap_dump_open().  Note that its calling arguments are suitable for use
       with  pcap_dispatch()  or  pcap_loop().   If  called directly, the user
       parameter is of type pcap_dumper_t as returned by pcap_dump_open().

       pcap_compile() is used to compile the string str into a filter program.
       program  is  a  pointer  to  a  bpf_program  struct and is filled in by
       pcap_compile().  optimize controls whether optimization on the  result-
       ing  code is performed.  netmask specifies the IPv4 netmask of the net-
       work on which packets are being captured; it is used only when checking
       for  IPv4 broadcast addresses in the filter program.  If the netmask of
       the network on which packets are being captured isn't known to the pro-
       gram, or if packets are being captured on the Linux "any" pseudo-inter-
       face that can capture on more than one network, a value  of  0  can  be
       supplied;  tests  for  IPv4 broadcast addreses won't be done correctly,
       but all other tests in the filter program will be OK.  A return  of  -1
       indicates  an  error in which case pcap_geterr() may be used to display
       the error text.

       pcap_compile_nopcap() is similar to pcap_compile() except that  instead
       of  passing  a  pcap  structure,  one  passes  the snaplen and linktype
       explicitly.  It is intended to be used for compiling filters for direct
       BPF  usage, without necessarily having called pcap_open().  A return of
       -1 indicates an error;  the  error  text  is  unavailable.   (pcap_com-
       pile_nopcap() is a wrapper around pcap_open_dead(), pcap_compile(), and
       pcap_close(); the latter three routines can be used directly  in  order
       to get the error text for a compilation error.)

       pcap_setfilter()  is used to specify a filter program.  fp is a pointer
       to a bpf_program struct, usually the result  of  a  call  to  pcap_com-
       pile().   -1 is returned on failure, in which case pcap_geterr() may be
       used to display the error text; 0 is returned on success.

       pcap_freecode() is used to free up allocated memory  pointed  to  by  a
       bpf_program struct generated by pcap_compile() when that BPF program is
       no longer needed, for example after it has been made the filter program
       for a pcap structure by a call to pcap_setfilter().

       pcap_setdirection() is used to specify a direction that packets will be
       captured.   pcap_direction_t  is  one  of  the   constants   PCAP_D_IN,
       PCAP_D_OUT  or  PCAP_D_INOUT.   PCAP_D_IN  will  only  capture  packets
       received by the device, PCAP_D_OUT will only capture  packets  sent  by
       the device and PCAP_D_INOUT will capture packets received by or sent by
       the device.  PCAP_D_INOUT is the default setting if  this  function  is
       not  called.   This  isn't necessarily supported on all platforms; some
       platforms might return an error, and some  other  platforms  might  not
       support  PCAP_D_OUT.  This operation is not supported if a ``savefile''
       is being read.  -1 is returned on failure, 0 is returned on success.

       pcap_datalink() returns the link layer type; link layer  types  it  can
       return include:


            DLT_NULL
                 BSD loopback encapsulation; the link layer header is a 4-byte
                 field, in host  byte  order,  containing  a  PF_  value  from
                 socket.h for the network-layer protocol of the packet.

                 Note  that  ``host  byte  order''  is  the  byte order of the
                 machine on which the packets are captured, and the PF_ values
                 are  for  the OS of the machine on which the packets are cap-
                 tured; if a live capture is being done, ``host  byte  order''
                 is  the  byte order of the machine capturing the packets, and
                 the PF_ values are those of the OS of the  machine  capturing
                 the  packets,  but  if a ``savefile'' is being read, the byte
                 order and PF_ values are not necessarily those of the machine
                 reading the capture file.

            DLT_EN10MB
                 Ethernet (10Mb, 100Mb, 1000Mb, and up)

            DLT_IEEE802
                 IEEE 802.5 Token Ring

            DLT_ARCNET
                 ARCNET

            DLT_SLIP
                 SLIP; the link layer header contains, in order:

                      a  1-byte  flag,  which is 0 for packets received by the
                      machine and 1 for packets sent by the machine;

                      a 1-byte field, the upper 4 bits of which  indicate  the
                      type of packet, as per RFC 1144:

                           0x40 an unmodified IP datagram (TYPE_IP);

                           0x70 an   uncompressed-TCP   IP   datagram  (UNCOM-
                                PRESSED_TCP), with that byte being  the  first
                                byte  of  the  raw IP header on the wire, con-
                                taining the connection number in the  protocol
                                field;

                           0x80 a compressed-TCP IP datagram (COMPRESSED_TCP),
                                with that byte being the  first  byte  of  the
                                compressed TCP/IP datagram header;

                      for  UNCOMPRESSED_TCP,  the  rest  of  the  modified  IP
                      header, and for COMPRESSED_TCP,  the  compressed  TCP/IP
                      datagram header;

                 for a total of 16 bytes; the uncompressed IP datagram follows
                 the header.

            DLT_PPP
                 PPP; if the first 2 bytes are 0xff  and  0x03,  it's  PPP  in
                 HDLC-like  framing,  with  the PPP header following those two
                 bytes, otherwise it's PPP without  framing,  and  the  packet
                 begins with the PPP header.

            DLT_FDDI
                 FDDI

            DLT_ATM_RFC1483
                 RFC 1483 LLC/SNAP-encapsulated ATM; the packet begins with an
                 IEEE 802.2 LLC header.

            DLT_RAW
                 raw IP; the packet begins with an IP header.

            DLT_PPP_SERIAL
                 PPP in HDLC-like framing, as per RFC 1662, or Cisco PPP  with
                 HDLC  framing,  as  per  section 4.3.1 of RFC 1547; the first
                 byte will be 0xFF for PPP in HDLC-like framing, and  will  be
                 0x0F or 0x8F for Cisco PPP with HDLC framing.

            DLT_PPP_ETHER
                 PPPoE;  the  packet  begins  with  a PPPoE header, as per RFC
                 2516.

            DLT_C_HDLC
                 Cisco PPP with HDLC framing, as  per  section  4.3.1  of  RFC
                 1547.

            DLT_IEEE802_11
                 IEEE 802.11 wireless LAN

            DLT_FRELAY
                 Frame Relay

            DLT_LOOP
                 OpenBSD  loopback  encapsulation;  the link layer header is a
                 4-byte field, in network byte order, containing a  PF_  value
                 from OpenBSD's socket.h for the network-layer protocol of the
                 packet.

                 Note that, if a ``savefile'' is being read, those PF_  values
                 are  not necessarily those of the machine reading the capture
                 file.

            DLT_LINUX_SLL
                 Linux "cooked" capture encapsulation; the link  layer  header
                 contains, in order:

                      a  2-byte "packet type", in network byte order, which is
                      one of:

                           0    packet was sent to us by somebody else

                           1    packet was broadcast by somebody else

                           2    packet was multicast, but  not  broadcast,  by
                                somebody else

                           3    packet  was  sent by somebody else to somebody
                                else

                           4    packet was sent by us

                      a 2-byte field, in  network  byte  order,  containing  a
                      Linux ARPHRD_ value for the link layer device type;

                      a  2-byte  field,  in network byte order, containing the
                      length of the link layer address of the  sender  of  the
                      packet (which could be 0);

                      an  8-byte  field containing that number of bytes of the
                      link layer header (if there are more than 8 bytes,  only
                      the first 8 are present);

                      a  2-byte field containing an Ethernet protocol type, in
                      network byte order, or containing  1  for  Novell  802.3
                      frames  without  an  802.2  LLC  header  or 4 for frames
                      beginning with an 802.2 LLC header.

            DLT_LTALK
                 Apple LocalTalk; the packet begins  with  an  AppleTalk  LLAP
                 header.

            DLT_PFLOG
                 OpenBSD pflog; the link layer header contains, in order:

                      a 1-byte header length, in host byte order;

                      a 4-byte PF_ value, in host byte order;

                      a  2-byte  action  code, in network byte order, which is
                      one of:

                           0    passed

                           1    dropped

                           2    scrubbed

                      a 2-byte reason code, in network byte  order,  which  is
                      one of:

                           0    match

                           1    bad offset

                           2    fragment

                           3    short

                           4    normalize

                           5    memory

                      a 16-character interface name;

                      a  16-character ruleset name (only meaningful if subrule
                      is set);

                      a 4-byte rule number, in network byte order;

                      a 4-byte subrule number, in network byte order;

                      a 1-byte direction, in network byte order, which is  one
                      of:

                           0    incoming or outgoing

                           1    incoming

                           2    outgoing

            DLT_PRISM_HEADER
                 Prism  monitor mode information followed by an 802.11 header.

            DLT_IP_OVER_FC
                 RFC 2625 IP-over-Fibre Channel, with  the  link-layer  header
                 being the Network_Header as described in that RFC.

            DLT_SUNATM
                 SunATM devices; the link layer header contains, in order:

                      a  1-byte flag field, containing a direction flag in the
                      uppermost bit, which is set for packets  transmitted  by
                      the  machine  and  clear  for  packets  received  by the
                      machine, and a 4-byte traffic type in  the  low-order  4
                      bits, which is one of:

                           0    raw traffic

                           1    LANE traffic

                           2    LLC-encapsulated traffic

                           3    MARS traffic

                           4    IFMP traffic

                           5    ILMI traffic

                           6    Q.2931 traffic

                      a 1-byte VPI value;

                      a 2-byte VCI field, in network byte order.

            DLT_IEEE802_11_RADIO
                 link-layer  information  followed  by  an 802.11 header - see
                 http://www.shaftnet.org/~pizza/software/capturefrm.txt for  a
                 description of the link-layer information.

            DLT_ARCNET_LINUX
                 ARCNET,  with no exception frames, reassembled packets rather
                 than raw frames, and an extra 16-bit offset field between the
                 destination host and type bytes.

            DLT_LINUX_IRDA
                 Linux-IrDA  packets,  with a DLT_LINUX_SLL header followed by
                 the IrLAP header.

            DLT_LINUX_LAPD
                 LAPD (Q.921) frames, with a DLT_LINUX_SLL header captured via
                 vISDN.

       pcap_list_datalinks()  is used to get a list of the supported data link
       types  of  the  interface  associated   with   the   pcap   descriptor.
       pcap_list_datalinks()  allocates  an  array  to  hold the list and sets
       *dlt_buf.  The caller is responsible for  freeing  the  array.   -1  is
       returned  on  failure;  otherwise, the number of data link types in the
       array is returned.

       pcap_set_datalink() is used to set the current data link  type  of  the
       pcap  descriptor to the type specified by dlt.  -1 is returned on fail-
       ure.

       pcap_datalink_name_to_val() translates a data link type name, which  is
       a  DLT_ name with the DLT_ removed, to the corresponding data link type
       value.  The translation is case-insensitive.  -1 is returned  on  fail-
       ure.

       pcap_datalink_val_to_name()  translates  a  data link type value to the
       corresponding data link type name.  NULL is returned on failure.

       pcap_datalink_val_to_description() translates a data link type value to
       a  short description of that data link type.  NULL is returned on fail-
       ure.

       pcap_snapshot()   returns   the   snapshot   length   specified    when
       pcap_open_live() was called.

       pcap_is_swapped()  returns true if the current ``savefile'' uses a dif-
       ferent byte order than the current system.

       pcap_major_version() returns the major number of the file format of the
       savefile;  pcap_minor_version()  returns  the  minor number of the file
       format of the savefile.  The version number is stored in the header  of
       the savefile.

       pcap_file()  returns  the standard I/O stream of the ``savefile,'' if a
       ``savefile'' was opened with pcap_open_offline(), or NULL, if a network
       device was opened with pcap_open_live().

       pcap_stats()  returns 0 and fills in the pcap_stat structure pointed to
       by its second argument.  The values represent  packet  statistics  from
       the  start of the run to the time of the call.  If there is an error or
       the underlying packet capture doesn't support packet statistics, -1  is
       returned  and  the  error  text  can  be obtained with pcap_perror() or
       pcap_geterr().  pcap_stats() is supported only on live captures, not on
       ``savefiles''; no statistics are stored in ``savefiles'', so no statis-
       tics are available when reading from a ``savefile''.

       pcap_fileno() returns the file descriptor number  from  which  captured
       packets are read, if a network device was opened with pcap_open_live(),
       or -1, if a ``savefile'' was opened with pcap_open_offline().

       pcap_get_selectable_fd() returns, on UNIX, a file descriptor number for
       a  file descriptor on which one can do a select() or poll() to wait for
       it to be possible to read packets without blocking, if such a  descrip-
       tor  exists, or -1, if no such descriptor exists.  Some network devices
       opened with pcap_open_live() do not support  select()  or  poll()  (for
       example, regular network devices on FreeBSD 4.3 and 4.4, and Endace DAG
       devices), so -1 is returned for those devices.

       Note that on most versions of most BSDs (including Mac OS  X)  select()
       and    poll()    do    not    work    correctly    on    BPF   devices;
       pcap_get_selectable_fd() will return a file descriptor on most of those
       versions  (the exceptions being FreeBSD 4.3 and 4.4), a simple select()
       or  poll()  will  not  return  even  after  a  timeout   specified   in
       pcap_open_live()  expires.   To  work  around this, an application that
       uses select() or poll() to wait for packets  to  arrive  must  put  the
       pcap_t  in  non-blocking  mode,  and  must arrange that the select() or
       poll() have a timeout less than or equal to the  timeout  specified  in
       pcap_open_live(),  and  must  try  to  read  packets after that timeout
       expires, regardless of whether select() or poll()  indicated  that  the
       file  descriptor  for  the  pcap_t  is  ready to be read or not.  (That
       workaround will not work in FreeBSD 4.3 and later; however, in  FreeBSD
       4.6  and  later,  select() and poll() work correctly on BPF devices, so
       the workaround isn't necessary, although it does no harm.)

       pcap_get_selectable_fd() is not available on Windows.

       pcap_perror() prints the text of the last pcap library error on stderr,
       prefixed by prefix.

       pcap_geterr()  returns  the  error  text  pertaining  to  the last pcap
       library error.  NOTE: the pointer it returns will no longer point to  a
       valid error message string after the pcap_t passed to it is closed; you
       must use or copy the string before closing the pcap_t.

       pcap_strerror() is provided in case strerror(1) isn't available.

       pcap_lib_version() returns a pointer to  a  string  giving  information
       about  the version of the libpcap library being used; note that it con-
       tains more information than just a version number.

       pcap_close()  closes  the  files  associated  with  p  and  deallocates
       resources.

       pcap_dump_file()  returns  the  standard I/O stream of the ``savefile''
       opened by pcap_dump_open().

       pcap_dump_flush() flushes the output buffer  to  the  ``savefile,''  so
       that  any  packets  written with pcap_dump() but not yet written to the
       ``savefile'' will be written.  -1 is returned on error, 0 on success.

       pcap_dump_ftell() returns the current file  position  for  the  ``save-
       file'',  representing  the  number of bytes written by pcap_dump_open()
       and pcap_dump().  -1 is returned on error.

       pcap_dump_close() closes the ``savefile.''


SEE ALSO
       tcpdump(8), tcpslice(1)

AUTHORS
       The original authors are:

       Van Jacobson, Craig Leres and  Steven  McCanne,  all  of  the  Lawrence
       Berkeley National Laboratory, University of California, Berkeley, CA.

       The current version is available from "The Tcpdump Group"'s Web site at

              http://www.tcpdump.org/

BUGS
       Please send problems, bugs, questions, desirable enhancements, etc. to:

              tcpdump-workers@tcpdump.org

       Please send source code contributions, etc. to:

              patches@tcpdump.org



                               27 February 2004                        PCAP(3)

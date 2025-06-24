test     start   1000              test program for sic software
first    stl     retadr            save return address
cloop    jsub    rdrec             read input record
         lda     length            test for eof (length = 0
         comp    zero                eol == 1)
         jeq     endfil            exit if eof found
         jsub    wrrec             write output record
         j       cloop             loop
endfil   lda     zero              insert end of file marker
         sta     buffer
         lda     three             set length = 3
         sta     length
         jsub    wrrec             write eof
         ldl     retadr            get return address
         rsub                      return to caller
three    word    3
zero     word    0
one      word    1
k5       word    5
k11      word    11
retadr   resw    1
length   resw    1
.
cchar    resw    1                 character for comparison
ochar    resw    1                 character for out
.
buffer   resb    4096              4096-byte buffer area
.
dis      word    32
.
threa    word    64                A = 65, num < A - 1
threb    word    90                Z = 90, num > Z
threc    word    96                a = 97, num < a - 1
thred    word    122               z = 122, num > z
.
.       subroutine to read record into buffer
.
rdrec    ldx     zero              clear loop counter
         lda     zero              clear a to zero
rloop    td      input             test input device
         jeq     rloop             loop until ready
         rd      input             read character into register a
         comp    k11               test for eol or eof
         jlt     exit              exit loop if found
.
         sta     cchar
         lda     cchar
         sta     ochar
.
. A - 1 < uppercase <= Z < other characters <= a - 1 < lowercase <= z < other character
.
         comp    threa
         jgt     doa
.
goa      lda     cchar
         comp    threb
         jgt     dob
.
gob      lda     cchar
         comp    threc
         jgt     doc
.
goc      lda     cchar
         comp    thred
         jgt     dod
.
out      lda     ochar
         stch    buffer,x          store character in buffer
         tix     maxlen            loop unless max length
         jlt     rloop                 has been reached
exit     stch    buffer,x          store eol/eof in buffer
         stx     length            save record length
         comp    k5
         jlt     endrd
         lda     length            modify record length to include
         add     one                 eol
         sta     length
endrd    rsub                      return to caller
.
doa      add     dis
         sta     ochar
         j       goa
.
dob      sta     ochar
         j       gob
.
doc      sub     dis
         sta     ochar
         j       goc
.
dod      sta     ochar
         j       out
.
input    byte    x'f3'             code for input device
maxlen   word    4096
.
.       subroutine to write record from buffer
.
wrrec    ldx     zero              clear loop counter
wloop    td      output            test output device
         jeq     wloop             loop until ready
         ldch    buffer,x          get character from buffer
         wd      output            write character
         tix     length            loop until all characters
         jlt     wloop                have been written
         rsub                      return to caller
output   byte    x'06'             code for output device
         end     first
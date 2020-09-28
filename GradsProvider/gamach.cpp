#include "gamach.h"

static char masks[8] = {0,127,63,31,15,7,3,1};

/*  32-bit gagby */
gaint gagby (unsigned char *ch, gaint ioff, gaint ilen) {
gaint ival;
unsigned char *ch1;

  ch1 = (unsigned char *)(&ival);
  ival = 0;
  if (BYTEORDER) {
    if (ilen==1) *(ch1+3) = *(ch+ioff);
    else if (ilen==2) {
      *(ch1+2) = *(ch+ioff);
      *(ch1+3) = *(ch+ioff+1);
    } else if (ilen==3) {
      *(ch1+1) = *(ch+ioff);
      *(ch1+2) = *(ch+ioff+1);
      *(ch1+3) = *(ch+ioff+2);
    } else {
      *ch1 = *(ch+ioff);
      *(ch1+1) = *(ch+ioff+1);
      *(ch1+2) = *(ch+ioff+2);
      *(ch1+3) = *(ch+ioff+3);
    }
  } else {
    if (ilen==1) *ch1 = *(ch+ioff);
    else if (ilen==2) {
      *(ch1+1) = *(ch+ioff);
      *ch1 = *(ch+ioff+1);
    } else if (ilen==3) {
      *(ch1+2) = *(ch+ioff);
      *(ch1+1) = *(ch+ioff+1);
      *ch1 = *(ch+ioff+2);
    } else {
      *(ch1+3) = *(ch+ioff);
      *(ch1+2) = *(ch+ioff+1);
      *(ch1+1) = *(ch+ioff+2);
      *ch1 = *(ch+ioff+3);
    }
  }
  return (ival);
}

/* 32-bit gagbb */

gaint gagbb (unsigned char *ch, gaint ioff, gaint ilen) {
gaint ival,istrt,iend,cstrt;
gaint i,ispac,ioff2,ileav,numb;
unsigned char *ch1;

  ch1 = (unsigned char *)(&ival);
  ival = 0;
  istrt = ioff/8;
  ispac = ioff - istrt*8;
  /* Fast path for byte alignment */
  if (ispac==0 && (ilen==8 || ilen==16 || ilen==24 || ilen==32)) {
    if (BYTEORDER) {
      if (ilen==8) *(ch1+3) = *(ch+istrt);
      else if (ilen==16) {
        *(ch1+2) = *(ch+istrt);
        *(ch1+3) = *(ch+istrt+1);
      } else if (ilen==24) {
        *(ch1+1) = *(ch+istrt);
        *(ch1+2) = *(ch+istrt+1);
        *(ch1+3) = *(ch+istrt+2);
      } else {
        *ch1 = *(ch+istrt);
        *(ch1+1) = *(ch+istrt+1);
        *(ch1+2) = *(ch+istrt+2);
        *(ch1+3) = *(ch+istrt+3);
      }
    } else {
      if (ilen==8) *ch1 = *(ch+istrt);
      else if (ilen==16) {
        *(ch1+1) = *(ch+istrt);
        *ch1 = *(ch+istrt+1);
      } else if (ilen==24) {
        *(ch1+2) = *(ch+istrt);
        *(ch1+1) = *(ch+istrt+1);
        *ch1 = *(ch+istrt+2);
      } else {
        *(ch1+3) = *(ch+istrt);
        *(ch1+2) = *(ch+istrt+1);
        *(ch1+1) = *(ch+istrt+2);
        *ch1 = *(ch+istrt+3);
      }
    }
    return (ival);
  }
  /* Do it the hard way */
  ioff2 = ioff+ilen-1;
  iend = ioff2/8;
  ileav = (iend+1)*8 - ioff2;
  numb = iend - istrt;
  if (BYTEORDER) {
    cstrt = 3-numb;
    if (ispac>0) *(ch1+cstrt) = *(ch+istrt) & masks[ispac];
    else *(ch1+cstrt) = *(ch+istrt);
    for (i=1; i<=numb; i++) {
      *(ch1+cstrt+i) = *(ch+istrt+i);
    }
  } else {
    if (ispac>0) *(ch1+numb) = *(ch+istrt) & masks[ispac];
    else *(ch1+numb) = *(ch+istrt);
    for (i=0; i<numb; i++) {
      *(ch1+i) = *(ch+iend-i);
    }
  }
  ival = ival >> (ileav-1);
  return (ival);
}

/* 32-bit gapby
  routine to put an integer length one to four into a char stream */

void gapby (gaint ival, unsigned char *ch, gaint ioff, gaint ilen) {
unsigned char *ch1;

  ch1 = (unsigned char *)(&ival);
  if (BYTEORDER) {
    if (ilen==1) *(ch+ioff) = *(ch1+3);
    else if (ilen==2) {
      *(ch+ioff) = *(ch1+2);
      *(ch+ioff+1) = *(ch1+3);
    } else if (ilen==3) {
      *(ch+ioff) = *(ch1+1);
      *(ch+ioff+1) = *(ch1+2);
      *(ch+ioff+2) = *(ch1+3);
    } else {
      *(ch+ioff) = *ch1;
      *(ch+ioff+1) = *(ch1+1);
      *(ch+ioff+2) = *(ch1+2);
      *(ch+ioff+3) = *(ch1+3);
    }
  } else {
    if (ilen==1)  *(ch+ioff) = *ch1;
    else if (ilen==2) {
      *(ch+ioff) = *(ch1+1);
      *(ch+ioff+1) = *ch1 ;
    } else if (ilen==3) {
      *(ch+ioff) = *(ch1+2) ;
      *(ch+ioff+1) = *(ch1+1) ;
      *(ch+ioff+2) = *ch1 ;
    } else {
      *(ch+ioff) = *(ch1+3) ;
      *(ch+ioff+1) = *(ch1+2) ;
      *(ch+ioff+2) = *(ch1+1) ;
      *(ch+ioff+3) = *ch1;
    }
  }
  return;
}

/* 32-bit gapbb 

  Put bit string value into a character string 
  starting at bit offset ioff and number of bits ilen.  
  ilen should not be greater than 24 bits unless byte aligned.  
  Note that ch cannot be longer than 32K on a PC. */

void gapbb (gaint ival, unsigned char *ch, gaint ioff, gaint ilen) {
unsigned char *ch1;
gaint istrt,iend,cstrt;
gaint i,ispac,ioff2,ileav,numb;

  ch1 = (unsigned char *)(&ival);
  istrt = ioff/8;
  ispac = ioff - istrt*8;
  /* Fast path for byte alignment */
  if (ispac==0 && (ilen==8 || ilen==16 || ilen==24 || ilen==32)) {
    if (BYTEORDER) {
      if (ilen==8) *(ch+istrt) = *(ch1+3);
      else if (ilen==16) {
        *(ch+istrt)   = *(ch1+2) ;
        *(ch+istrt+1) = *(ch1+3);
      } else if (ilen==24) {
        *(ch+istrt)   = *(ch1+1);
        *(ch+istrt+1) = *(ch1+2);
        *(ch+istrt+2) = *(ch1+3);
      } else {
        *(ch+istrt)   = *ch1;
        *(ch+istrt+1) = *(ch1+1);
        *(ch+istrt+2) = *(ch1+2);
        *(ch+istrt+3) = *(ch1+3);
      }
    } else {
      if (ilen==8) *(ch+istrt) = *ch1;
      else if (ilen==16) {
        *(ch+istrt)   = *(ch1+1);
        *(ch+istrt+1) = *ch1;
      } else if (ilen==24) {
        *(ch+istrt)   = *(ch1+2);
        *(ch+istrt+1) = *(ch1+1);
        *(ch+istrt+2) = *ch1;
      } else {
        *(ch+istrt)   = *(ch1+3);
        *(ch+istrt+1) = *(ch1+2);
        *(ch+istrt+2) = *(ch1+1);
        *(ch+istrt+3) = *ch1;
      }
    }
    return;
  }
  /* Do it the hard way */
  ioff2 = ioff+ilen-1;
  iend = ioff2/8;
  ileav = (iend+1)*8 - ioff2;
  numb = iend - istrt;
  ival <<= (ileav-1);
  if (BYTEORDER) {
    cstrt = 3-numb;
    if (ispac>0) *(ch+istrt) |= *(ch1+cstrt) ;
    else *(ch+istrt) = *(ch1+cstrt);
    for (i=1; i <= numb; i++) {
      *(ch+istrt+i) = *(ch1+cstrt+i);
    }
  } else {
    if (ispac>0) *(ch+istrt) |= *(ch1+numb) ;
    else *(ch+istrt) = *(ch1+numb);
    for (i=0; i<numb; i++) {
      *(ch+iend-i) = *(ch1+i);
    }
  }
  return;
}

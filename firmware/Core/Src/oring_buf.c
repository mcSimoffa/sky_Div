#include <stdlib.h>
#include "oring_buf.h"

#define BUF_SIZE  32
uint32_t  bf[BUF_SIZE];
uint32_t  *head = bf;
uint32_t  *tail = bf;

//-------------------------------------------
static uint32_t *next_addr(uint32_t *addr)
{
  if (++addr >= bf+BUF_SIZE)
    addr -= BUF_SIZE;
  return (addr);
}

//-------------------------------------------
bool oring_buf_put(uint32_t item)
{
  bool retval = false;
  if(next_addr(head)!=tail)
  {
    *head = item;
    head =  next_addr(head);
    retval = true;
  }
  return (retval);
}

//-------------------------------------------
bool oring_buf_get(uint32_t *item)
{
  bool retval = false;
  if(head!=tail)
  {
    *item = *tail;
    tail =  next_addr(tail);
    retval = true;
  }
  return (retval);
}

#include "tinyos.h"
#include "tinyos.h"
#include "kernel_streams.h"
#include "kernel_sched.h"
#include "kernel_cc.h"
#include "kernel_socket.h"


socket_cb* PORT_MAP[MAX_PORT+1];

static file_ops socket_file_ops ={
  .Open=NULL,
  .Read=useless,
  .Write=pipe_write,
  .Close=pipe_writer_close
};

Fid_t sys_Socket(port_t port)
{
  if(port<NOPORT || port>MAX_PORT){
  	return -1;
  }

  Fid_t fid[1];//new file id 
  FCB* fcb[1]; //new file control block 
    
  if (FCB_reserve(1,fid,fcb)==0) {
    return -1;
  }

  socket_cb* scb=xmalloc(sizeof(socket_cb));
  scb->refcount=0;
  scb->fcb=fcb[0];
  scb->fid=fid[0];
  scb->type=SOCKET_UNBOUND;
  scb->port=port;

  fcb[0]->streamobj=scb;
  fcb[0]->streamfunc=&socket_file_ops;
  
  return (Fid_t) scb->fid;
}

int sys_Listen(Fid_t sock)
{
	return -1;
}


Fid_t sys_Accept(Fid_t lsock)
{
	return NOFILE;
}


int sys_Connect(Fid_t sock, port_t port, timeout_t timeout)
{
	return -1;
}


int sys_ShutDown(Fid_t sock, shutdown_mode how)
{
	return -1;
}
int socket_write(void* this, const char *buf, unsigned int n){
  socket_cb* scb=(socket_cb*)this;
  assert(scb!=NULL);

  //scb->type=SOCKET_PEER;
  peer_socket* peer_s=scb->peer_s;
  pipe_cb* p=peer_s->read_pipe;
  pipe_write(p,buf,n);

  return 0;
}

int socket_read(void* this, char *buf, unsigned int n){
  socket_cb* scb=(socket_cb*)this;
  assert(scb!=NULL);

  //scb->type=SOCKET_PEER;
  peer_socket* peer_s=scb->peer_s;
  pipe_cb* p=peer_s->write_pipe;
  pipe_read(p,buf,n);

  return 0;
}

int socket_close(void* this){
	return -1;
}


#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"
#include "readmap.h"
// Arminius 8.4 manor
#include "family.h"

static void NPC_SignBoard_Window( int meindex, int toindex, int num,int select);


/*********************************
* 初期処  
*********************************/
BOOL NPC_SignBoardInit( int meindex )
{

	char npcarg1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *npcarg;
	/*--タイプ設定--*/
   	CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPEMSG );

	/*--ファイルがなければ�禺�  を設  させない--*/
	if((npcarg = NPC_Util_GetArgStr( meindex, npcarg1, sizeof(npcarg1)))==NULL){
		print("GetArgStrErr");
		return FALSE;
	}

	return TRUE;

}




/*********************************
* 見られた時の処  
*********************************/
void NPC_SignBoardLooked( int meindex , int lookedindex)
{

    /* プレイヤーに対してだけ  応する */
    if( CHAR_getInt( lookedindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) {
    	return;
    }

	/* １グリッド以  のみ */
	if( NPC_Util_CharDistance( lookedindex, meindex ) > 1) return;


	NPC_SignBoard_Window( meindex, lookedindex,0,-1);

}


static void NPC_SignBoard_Window( int meindex, int toindex, int num,int select)
{

	char npcarg1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *npcarg;
	char token[1024];
	int fd = getfdFromCharaIndex( toindex);


	if((npcarg = NPC_Util_GetArgStr( meindex, npcarg1, sizeof(npcarg1)))==NULL){
		print("GetArgStrErr");
		return;
	}
	
	// Arminius: manor
	{
	  char *p,*q;
	  extern struct FM_POINTLIST  fmpointlist;
	  p=strstr(npcarg, "%manorid:");
	  if (p!=NULL) {
	    char tmp[1024];
	    int a;
	    
	    q=strstr(p+1,"%");
	    if (q!=NULL) {
  	      q[0]='\0';
 	      p[0]='\0';
	      a=atoi(p+9);
#ifdef _FAMILY_MANORNUM_CHANGE	// CoolFish ノ�哭廡鏝剖藜洞q
	      if ((a>=1) && (a<=MANORNUM)) {		  
#else
	      if ((a>=1) && (a<=4)) {
#endif
                getStringFromIndexWithDelim(fmpointlist.pointlistarray[a-1], "|", 5, token, sizeof(token));
                if (atoi(token)>=0) {
                  getStringFromIndexWithDelim(fmpointlist.pointlistarray[a-1], "|", 6, token, sizeof(token));
  	          sprintf(tmp,"%s%s%s",npcarg,token,q+1);
	          //strcpy(npcarg, tmp);
	          sprintf(token, "�@�@�@�@�@�@》�@�櫑O�@「\n%s",tmp);
	        } else {
	          sprintf(tmp,"%s�SΤヴ�鶤a謁%s",npcarg,q+1);
	          //strcpy(npcarg, tmp);
	          sprintf(token, "�@�@�@�@�@�@》�@�櫑O�@「\n%s",tmp);
	        }
	      }
	    }
	    
	  }else{
	  	sprintf(token, "�@�@�@�@�@�@》�@�櫑O�@「\n%s",npcarg);
	  }
	}

	
	
	/*--送信--*/
	lssproto_WN_send( fd,WINDOW_MESSAGETYPE_MESSAGE,
					WINDOW_BUTTONTYPE_OK,
					CHAR_WINDOWTYPE_WINDOWPETSHOP_START,
					CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);
}



/*-----------------------------------------
 * クライアントから返ってきた時に呼び出される�e
 *
-------------------------------------------*/
void NPC_SignBoardWindowTalked( int meindex, int talkerindex, 
								int seqno, int select, char *data)
{
	
	if( NPC_Util_CharDistance( talkerindex, meindex ) > 1) {
		return;
	}
}


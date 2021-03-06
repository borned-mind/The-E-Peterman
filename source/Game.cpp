#include "Game.hpp"
#include"Util.hpp"
#include"graph.h"




#include <GL/gl.h>
#include <GL/glu.h>
#include"menu.hpp"

#if defined(_WIN32) || defined(_WIN64)
	#include<time.h>
#endif



#define REVERSESTRING(str){\
	char * tmpStr = malloc(GameDrive::Crypto::Rotor::_strlen_(str));\
	void * ftmpStr=tmpStr;\
	void *fStr = str;\
	while(*str) *tmpStr++=*str++;\
	str = fStr;\
	while(tmpStr != ftmpStr) *str++=*tmpStr--;\
	free(tmpStr);\
}

#define COTENATION(array, phrase, sizeArray){\
	if(sizeArray < GameDrive::Crypto::Rotor::_strlen_(phrase) > sizeArray){\
		sizeArray += GameDrive::Crypto::Rotor::_strlen_(phrase) - sizeArray;\
		array = (char*)realloc(array, sizeArray);\
	}\
		{\
		register unsigned long i = 0;\
		puts(phrase);\
		while(*array) *array++;\
		while(phrase[i]) *array++ = (phrase[i++]);\
		}\
}

namespace GameDrive{



namespace Crypto{
	namespace Numbers{

		char * convertNumberToPhrases(unsigned long long number){
			unsigned long alloced, used=0;
		
			alloced = number/10 + 1;

			char * result = (char*)calloc(sizeof(ull)*alloced,1);
			void * fresult = result;
			register unsigned char num;
			while(number){
				//printf("%s\n",(char*) fresult);
				if( used + 1 > alloced ){
					result = (char*)realloc(result, alloced+1);
					alloced+=1;
				}
				num = number % 10;
				switch(num){
					case 0:
						COTENATION(result, "zero", alloced);
						break;
					case 1:
						COTENATION(result, "one", alloced);
						break;
					case 2:
						COTENATION(result, "two", alloced);
						break;
					case 3:
						COTENATION(result, "three", alloced);
						break;
					case 4:
						COTENATION(result, "four", alloced);
						break;
					case 5:
						COTENATION(result, "five", alloced);
						break;
					case 6:
						COTENATION(result, "six", alloced);
						break;
					case 7:
						COTENATION(result, "seven", alloced);
						break;
					case 8:
						COTENATION(result, "eight", alloced);
						break;
					case 9:
						COTENATION(result, "nine", alloced);
						break;
					default:
						break;
				}
				used++;
				number/=10;

			}

			while(*result)*result++;

			*result='\0';

			return (char*)fresult;
		}
	};

	namespace Rotor{
		unsigned long _strlen_( const char * p){
			unsigned long r = 0;
			while(*p++) r++;
			return r;
		}

		template <class T> void shuffle(T * arr, unsigned int index0, unsigned int index1){
				//printf("%d %d\n",index0,index1);
				T tmp = arr[index0];
				arr[index0]=arr[index1];
				arr[index1]=tmp;
				//printf("Shuffle: %s\n", arr);
		}


		char * RotoPhrase(const char * what, unsigned char count){
			unsigned long sphrase = _strlen_(what);
			char * result = (char*)malloc( sphrase+1 ); // char 1 byte, then sizeof don't need
			void * fresult = result;
			while( *what )
				if(*what == ' ') {*what++;continue;}
				else *result++=*what++;// Copy full.
			*result++='\0';
			result=(char*)fresult;
			register unsigned int tmpi;		
			printf("Copy: %s\n",result);	
			for(count;count--;){
				
				tmpi= ( rand()%sphrase ) + 1 ;
				
				if( !result[tmpi]  ){
					count++;
					continue;
				}
				if(tmpi - 1 < 0)
				 shuffle<char>(result, tmpi, tmpi+1);
				else if(tmpi + 1 < sphrase)
				 shuffle<char>(result, tmpi, tmpi-1);	
				else
				{
					count++;
					continue;
				}
			}
			return result;
			
		}
	};
};


/*
namespace FreeType{
FT_Face InitFace(const char * fontName, unsigned short height){
 FT_Library library;
 if (FT_Init_FreeType( &library ))
	{
		fprintf(stderr, "Error with FreeType init library\n");
		GAMEEXIT;
	}
 	FT_Face face;
	if (FT_New_Face( library, fontName, 0, &face )) {
		fprintf(stderr, "Error with FreeType New Face\n");
		GAMEEXIT;
	}
	return face;
}

};
*/




const std::string difficulty[DifficultyCount]{
"noob","normal","hard"
};

unsigned char dif;

const std::string NameKeys[CountGameKey]{
"Forward",
"Left",
"Down",
"Right",
"Action",
"Jump"
};

std::string GameKeys[CountGameKey] = {
"W", // Top
"A", // Left
"S", // Bottom
"D", // Right
"E", // Action
"Space" // Jump
};

void Drive::InitIL(void){
  	ilInit();
	if (ilGetError() != IL_NO_ERROR) 
	  fprintf (stderr,"Devil Error (ilInit)");
	ilClearColour( 255, 255, 255, 000 );
}

Drive::Drive(void){
	this->InitKeys();
	this->InitIL();
	this->Phrase=0;
	this->WinRoom = 0;
}

void Drive::InitKeys(void){
	const char * Keys = Util::ReadFile(KEYSPATH);
	//printf("%s\n",Keys);
	void * fKeys = (void *)Keys;
	std::string KeyValue, KeyName;
	bool IsValue = true;
	while(*Keys){
		if(*Keys == '\n'){
		//	printf("%s : %s\n",KeyValue.c_str(),KeyName.c_str());
			*Keys++;
			IsValue=true;
			for(unsigned char i = CountGameKey-1;i--;){
				if(NameKeys[i]==KeyName)
					GameKeys[i]=KeyValue;
			}
			KeyValue="";
			KeyName="";
			continue;
		}
		if(*Keys==':'){*Keys++;IsValue=false;continue;}
		if(IsValue)
		 KeyValue += *Keys++;//operator overloading thx
		else
		  KeyName += *Keys++;
	}

	free(fKeys);
}

static inline void DrawDoors(GLdouble x, GLdouble y, GLdouble z){


		glLoadIdentity();
		
		glMatrixMode(GL_PROJECTION);

		glTranslatef(x,y,z);
		glMatrixMode(GL_MODELVIEW);
		




		glPushMatrix();
			glCallList(2);
		glPopMatrix();
	
}

static inline void InitRoom(void){

	GLuint TextureID = Util::textures::GL::LoadTexture("textures/floor.tga");
	glBindTexture( GL_TEXTURE_2D, TextureID );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glPolygonMode(GL_FRONT, GL_LINE);


	
	//Room
	glNewList(1, GL_COMPILE);
		glBegin(GL_QUADS);
			glColor3f(.33,.666,.9);
			//left
			/*
			The current texture coordinates are part of the data that is associated with each vertex and with the current raster position. Initially, the values for s, t, r, and q are (0, 0, 0, 1).

			*/
			//front
			glTexCoord3f( 0, 0, -1); 
			glVertex3f(0, 0, -1);

			glTexCoord3f( 1.0f, 0.0f, -1 );
			glVertex3f( 1.0f, 0.0f, -1);

			glTexCoord3f( 1.0f, 1.0f, 1 );
			glVertex3f(1.0f, 1.0f, 1);

			glTexCoord3f( 0.0f, 1.0f, 1 );
			glVertex3f(0.0f, 1.0f, 1);	

			//left
			glColor4ub(255,0,0,255);		
			glVertex3f(0, 1, 0);glTexCoord3f( -2, 1, 0 );	
			glVertex3f(-1, 2, 0);glTexCoord3f( 2, 2, 0 );
			glVertex3f(-1, -1, 1);glTexCoord3f( 2, -1, 1 );
			glVertex3f(0, 0, 1);glTexCoord3f( -2, 0, 1 );
	
			//bottom
		
			glColor3ub(0,0,255);

			glVertex3f(0, 0, -1); glTexCoord3f(0,0,-1);
			glVertex3f(1, 0, -1); glTexCoord3f(1,0,-1);

			glVertex3f(2, -1, 1); glTexCoord3f(2,-1,1);						
			glVertex3f(-1, -1, 1); glTexCoord3f(-1,-1,1);


			glColor3ub(0,255,0);
			
			//right
		       	glVertex3f(1, 0, 0); glTexCoord3f(0,0,1);
			glVertex3f(2, -1, 1); glTexCoord3f(1,-0,1);

			glVertex3f(2, 2, 1); glTexCoord3f(2,-1,1);
			glVertex3f(1, 1, 0); glTexCoord3f(-1,-1,0);		
				
		glEnd();
/*
		glBegin(GL_LINES);
						glColor4ub(255,0,0,255);			
						glVertex3f(0.0, 1, -1);

						glColor3ub(0,255,0);
						glVertex3f(-1, 2, 1);
			
						glColor3ub(255,0,255);
									
						glVertex3f(0, 0, -1);
						glVertex3f(-1, -1, 1);
						
						glVertex3f(1, 0, 0);
						glVertex3f(2, -1, 1);

						glVertex3f(1, 1, 0);
						glVertex3f(2, 2, 1);
						

			

		glEnd();
*/
	glEndList();
	//Doors
	glNewList(2,GL_COMPILE);
		glBegin(GL_QUADS);
		glColor3ub(0,0,0);
			
			//first Door

			glVertex3f(0,0,0);
			glVertex3f(0.25,0,1);
			glVertex3f(0.25,-0.5,1);
			glVertex3f(0,-0.5,1);
			
			//Two Door

			glVertex3f(1.0, 0, 1);
			glVertex3f(0.8, 0, 1);

			glVertex3f(0.8, -0.8, 1);
			glVertex3f(1, -1, 1);
			
			// three door
			glVertex3f(-0.8, 0, 1);
			glVertex3f(-0.8, -0.8, 1);

			glVertex3f(-1.2, -1.2, 1);
			glVertex3f(-1.2, 0, 1);

		glEnd();
		glBegin(GL_LINES);
			glColor3f(1.0, 1.0, 1.0);
			//first Door
			glVertex3f(0.0, 0, 0);
			glVertex3f(0.25, 0, 1);
			
			glVertex3f(0.25, 0, 1);
			glVertex3f(0.25, -0.5, 1);
			
			glVertex3f(0, 0, 1);
			glVertex3f(0, -0.5, 1);


			//two Door
			glVertex3f(1.0, 0, 1);
			glVertex3f(0.8, 0, 1);

			glVertex3f(0.8, 0, 1);
			glVertex3f(0.8, -0.8, 1);
			//three door
			glVertex3f(-0.8, 0, 1);
			glVertex3f(-0.8, -0.8, 1);

			glVertex3f(-0.8, 0, 1);
			glVertex3f(-1.2, 0, 1);
			
			
		glEnd();
	glEndList();

	//Book
	glNewList(3,GL_COMPILE);
		glBegin(GL_LINES);
			glVertex3f(-0, -0.75, 1);
			glVertex3f(0.3, -0.75, 1);
			
			glVertex3f(0.3,-0.75,1);
			glVertex3f(0.3,-0.65,1);
		glEnd();

		glBegin(GL_QUADS);
		
			glVertex3f(-0, -0.65, 1);
			glVertex3f(0, -0.75, 1);

			glVertex3f(0.3, -0.75, 1);			
			glVertex3f(0.3,-0.65,1);

		glEnd();


			

	glEndList();
	

	ilDeleteImages  (1, &TextureID);

}
static inline void DrawTunnel(void){
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPushMatrix();
		glCallList(1);
	glPopMatrix();


}



static inline void DrawRoom(void){

	glClearColor(0.2, 0.4, 0.6, 0.5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glLoadIdentity();
	glScalef(1,1,1);
	glTranslatef(0.5,0.5,0);
	//glOrtho(0, w_w, h_w, 0, 2, -2);



	//glViewport(x,y,w,h);


	DrawTunnel();

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		DrawDoors(1,1,0);
		glCallList(3);
	glDisable(GL_BLEND);


	SDL_RenderPresent(mrenderer);
}

void Drive::InitLevels(void){

	srand(time(NULL));
	
	this->rooms = (ull*) malloc(sizeof(ull) * 3 * 3 * dif);;

	
	
	for( unsigned short i = (3 * 3 * (dif+1) ); i--; ){
		this->rooms[i] = (rand()%MAXNUMBERROTOR) + 1 ;
	}
	this->lRooms = &this->rooms[3 * 3 * (dif+1)] ; 
	puts("Levels inited");
	

}

void Drive :: OpenShipher(){


	if(!this->Phrase){
			const char * Number = GameDrive::Crypto::Numbers::convertNumberToPhrases(WinRoom) ;
			this->Phrase = GameDrive::Crypto::Rotor::RotoPhrase( Number );
			free((void*)Number);
	}
	

	ull CountLettersOfPhrase = GameDrive::Crypto::Rotor::_strlen_(Phrase);

	SDL_Rect * ButtonsCoords = new SDL_Rect[CountLettersOfPhrase];

	SDL_Surface ** WinRoomText =  (SDL_Surface **)calloc(CountLettersOfPhrase, sizeof(SDL_Surface));
	SDL_Rect paperRect = {w_w/4 + w_w/10, h_w/4, w_w/2,h_w/4};

	int ButtonspPadding = (w_w/3)/CountLettersOfPhrase;
	unsigned int selected=0;
	int actived=-1;


	
	







	//SDL_Surface * WinRoomText = TTF_RenderUTF8_Blended_Wrapped(GameFont, \
				Phrase, {0,0,0}, w_w/4 );

	//SDL_UpdateWindowSurface(m_window);
	#define ACTIVEDCOLOR {122,122,0}
	#define SELECTEDCOLOR {200,0,200}
	#define SIMPLYCOLOR {255,255,255}

	bool menu=true;
	SDL_Event e;
	while(menu){
	SDL_SetRenderDrawColor(mrenderer, 120,120,120, 255);

	SDL_RenderFillRect(mrenderer, NULL);

	SDL_SetRenderDrawColor(mrenderer, 124, 188, 226, 245);
	SDL_RenderFillRect(mrenderer, &paperRect);

		for(int i = CountLettersOfPhrase; i--;){

			const char  tmp[] = {Phrase[i],'\0'};
			if(i == actived)
			 	WinRoomText[i] = TTF_RenderUTF8_Blended(GameFont, tmp, ACTIVEDCOLOR);
			else if(i == selected)
				WinRoomText[i] = TTF_RenderUTF8_Blended(GameFont, tmp, SELECTEDCOLOR);
			else
				WinRoomText[i] = TTF_RenderUTF8_Blended(GameFont, tmp, SIMPLYCOLOR);
			//SDL_BlitSurface

			ButtonsCoords[i] = {(w_w/2)-(w_w/10)+ButtonspPadding*i,
				  h_w/4 ,
					 WinRoomText[i]->w, WinRoomText[i]->h};

			SDL_Texture * WinRoomLetter = SDL_CreateTextureFromSurface(mrenderer, WinRoomText[i]);
			SDL_RenderCopy(mrenderer, WinRoomLetter, NULL, &ButtonsCoords[i]);
			SDL_DestroyTexture(WinRoomLetter);	
			SDL_FreeSurface(WinRoomText[i]);
		}

	SDL_Surface * DoorNumbers[3]; 
	SDL_Texture * DoorNumbersT[3];
	for(unsigned short i = 3; i --;){
		//printf("%d\n",i);
		std::stringstream TextRoom = numToStr( *(this->rooms+i), i );
		DoorNumbers[i] =  TTF_RenderUTF8_Blended(GameFont, TextRoom.str().c_str() , SELECTEDCOLOR);
		DoorNumbersT[i] = SDL_CreateTextureFromSurface(mrenderer, DoorNumbers[i]);
	}
	
	
	
	
	for(unsigned short i = 3;i--;){
		SDL_Rect tmp = { 100 + w_w/3 * i + DoorNumbers[i]->w, h_w-350, DoorNumbers[i]->w, DoorNumbers[i]->h};
		SDL_RenderCopy(mrenderer,DoorNumbersT[i], NULL, &tmp);
	}

	SDL_RenderPresent(mrenderer);

	for(unsigned short i = 3;i--;){
		SDL_FreeSurface(DoorNumbers[i]);
		SDL_DestroyTexture(DoorNumbersT[i]);	
	}

	SDL_RenderPresent(mrenderer);
	SDL_PollEvent(&e);
	switch(e.type){ // event switch
		case SDL_QUIT:
			printf("Quit after %d ticks\n",e.quit.timestamp);
			GAMEEXIT;
			break;
		case SDL_MOUSEMOTION:
			for(unsigned short i = CountLettersOfPhrase;i--;){
				if( MOUSECHECK(e, motion, ButtonsCoords, i) ){
					selected = i;
					break;
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			for(unsigned short i = CountLettersOfPhrase;i--;){

				if( MOUSECHECK(e, motion, ButtonsCoords, i) ){
					selected = i;
					if(actived>0){
						Crypto::Rotor::shuffle(this->Phrase, actived,i);
						actived=-1;
					}
					else
						actived = i;
					break;
				}
			}
		case SDL_KEYDOWN: 
		 if(e.key.keysym.scancode != SDL_GetScancodeFromKey(e.key.keysym.scancode)){
			 std::string MenuKey = SDL_GetKeyName(e.key.keysym.sym);
			 printf("MENU KEY: %s\n", MenuKey.c_str());
			 if(MenuKey == "Escape") menu=false;
			 else if(MenuKey == "Return"){
				if(actived>0){
					Crypto::Rotor::shuffle(this->Phrase, actived,selected);
					actived=-1;
				}
				else
					actived=selected;
			 }else if(MenuKey == "A" || MenuKey == "Left") {//elseif
				if(selected)
					 selected--;
			}else if(MenuKey == "D" || MenuKey == "Right"){
			 	if(selected < CountLettersOfPhrase-1) 
					selected++;
			}//elseif
			
		  } //if (e.key...
			e.key.keysym.scancode=(SDL_Scancode)0;
			break;
		 default:
			break;
		}//switch
	}



	//for(ull i = CountLettersOfPhrase; i--;)\
		SDL_FreeSurface(WinRoomText[i]);





}


std::stringstream  Drive::numToStr(ull num, unsigned char element){
	std::stringstream ReturnString; // I am bored...



	switch(element){
		case 0:
			ReturnString << "←Left:";
			break;			
		case 1:
			ReturnString << "→Right:";
			break;				
		case 2:
			ReturnString << "↑Top:";		
			break;			
		default:
			break;
	}
	ReturnString << num;
	return ReturnString;
}

bool Drive::BossRoom(void){
	BossPass = BossPass ? : 1;
	while(BossPass < 1000) BossPass*=10;
	while(BossPass > 9999) BossPass/=10;

	std::string PassText;
	std::stringstream BossPassText;
	BossPassText << BossPass;
	unsigned char lives = 3;
	
while(1){
	SDL_Rect paperRect = {w_w/4 + w_w/10, h_w/4, w_w/2,h_w/4};



	
	while(PassText.size() < 4){
		SDL_SetRenderDrawColor(mrenderer, 120,120,120, 255);	
		SDL_RenderFillRect(mrenderer, NULL);
		SDL_SetRenderDrawColor(mrenderer, 124, 188, 226, 245);
		SDL_RenderFillRect(mrenderer, &paperRect);

		SDL_Surface * p = TTF_RenderUTF8_Blended_Wrapped(GameFont, PassText.c_str(), SELECTEDCOLOR,w_w);
		SDL_Texture * pt = SDL_CreateTextureFromSurface(mrenderer, p);

		SDL_Rect RectRender = {(w_w/2)-(w_w/10),
				  h_w/4 , SizeFont, SizeFont};

		SDL_FreeSurface(p);
		SDL_RenderCopy(mrenderer, pt, NULL, &RectRender);
		SDL_DestroyTexture(pt);
		SDL_RenderPresent(mrenderer);

		std::string tmp = Util::Buttons::GetButton();
		if(tmp == "Escape") StartMenu(true);
		else if(tmp.c_str()[0] < 48 || tmp.c_str()[0] > 57) continue;
		PassText += tmp;
	}	
	if(PassText == BossPassText.str()){
		// TODO: Win.
		return true;
	}else{
		printf("lives: %d\n");
		lives--;
		PassText="";
		if(!lives) return false;
	}
}
	
 
}

void Drive::StartGame(void){
	//this->BossRoom();
	bool GameOver = false;
	bool SimplyMoving = false;
	//Light work which materials...
	//glEnable(GL_LIGHTING);

	SDL_GLContext glcontext = SDL_GL_CreateContext(m_window); // CreateOpenGL Context
	SDL_GL_SetSwapInterval(1);	//Vertical Sync
	//glActiveTexture(GL_TEXTURE0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST); 
	glShadeModel(GL_SMOOTH);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	InitRoom();



			
//	SDL_SetRenderDrawColor(mrenderer, 30,40,30,20);
//	SDL_RenderClear(mrenderer);
//	SDL




	GLenum glError;
	
	std::string Button;
	
	if( !this->WinRoom ){
		WinRoom = *(this->rooms + (rand()%3) ); 
		puts("WinRoom inited");
		if( 
			(float(WinRoom)/float(3) - WinRoom) == 0 && 
			(float(WinRoom)/float(6) - WinRoom) != 0 ) this->BossPass+=WinRoom;
	}




	glMatrixMode(GL_PROJECTION);
	gluLookAt(
		1.0,1.0,0.0, // x,y,z eye  
		1.0,1.0,1.0, // Center 
		0,1,0);
	glMatrixMode(GL_MODELVIEW);

 //*this->rooms

	DrawRoom();

	glError = glGetError();
	if(glError != GL_NO_ERROR) printf("OpenGL error: %s\n", gluErrorString(glError));



	
	
//	SDL_GL_SwapWindow(m_window); // Swap Window

		Button = Util::Buttons::GetButton();
		if(Button == "Escape") StartMenu(true);
		else if(Button == GameKeys[Action]){
			OpenShipher();
		}
		else if(Button == GameKeys[Left]){
			if(this->WinRoom != *(rooms) ) GameOver=true;
			else SimplyMoving=true;
		}else if(Button == GameKeys[Right]){
				if(this->WinRoom != *(rooms+1) ) GameOver=true;
				else SimplyMoving=true;
	
		}else if(Button == GameKeys[Top]){
			if(this->WinRoom != *(rooms+2) ) GameOver=true;
			else SimplyMoving=true;
			
		}
	 
	//printf("%d != %d/%d/%d\n",this->WinRoom, *(rooms), *(rooms+1), *(rooms+2));
	SDL_GL_DeleteContext(glcontext); //Delete Context
	SDL_RenderClear(mrenderer);
	if(SimplyMoving){
			if( (rooms+3 < lRooms) ) rooms+=3;
			else{
				puts("Okey need boss door");
				if( !this->BossRoom() ){
					GameOver = true;
				}else{
					//TODO: Win.
					SDL_Surface * WinText =
					TTF_RenderUTF8_Blended(GameFont, "You are win!..." , {0,255,0});

	
					SDL_RenderClear(mrenderer);
					SDL_RenderFillRect(mrenderer,NULL);
					SDL_RenderPresent(mrenderer);

					Util::images::putimage(WinText, w_w/2 - 100, h_w/4, 0, 0, w_w, h_w);
					SDL_UpdateWindowSurface(m_window);

					Util::Buttons::GetButton();
					SDL_FreeSurface(WinText);
					exit(0);
					
				}

			}
			//... Boss room
			free(Phrase);
			Phrase=0;
			this->WinRoom=0;
			SDL_SetRenderDrawColor(mrenderer,0,0,0,0);
			SDL_RenderClear(mrenderer);
			SDL_RenderPresent(mrenderer);
			SDL_Delay(500);
	}
	if( !GameOver ){
		this->StartGame();
	}
	

}

}

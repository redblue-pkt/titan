#ifndef ID3_H
#define ID3_H

#define TMPID3PICJPG "/tmp/id3pic.jpg"
#define TMPID3PICPNG "/tmp/id3pic.png"

struct id3genre id3genrelist[] =
{
	{0 , "Blues"}, {1 , "Classic Rock"}, {2 , "Country"}, {3 , "Dance"}, {4 , "Disco"}, {5 , "Funk"}, {6 , "Grunge"}, {7 , "Hip-Hop"}, {8 , "Jazz"}, {9 , "Metal"}, {10 , "New Age"},
	{11 , "Oldies"}, {12 , "Other"}, {13 , "Pop"}, {14 , "R&B"}, {15 , "Rap"}, {16 , "Reggae"}, {17 , "Rock"}, {18 , "Techno"}, {19 , "Industrial"}, {20 , "Alternative"},
	{21 , "Ska"}, {22 , "Death Metal"}, {23 , "Pranks"}, {24 , "Soundtrack"}, {25 , "Euro-Techno"}, {26 , "Ambient"}, {27 , "Trip-Hop"}, {28 , "Vocal"}, {29 , "Jazz+Funk"}, {30 , "Fusion"},
	{31 , "Trance"}, {32 , "Classical"}, {33 , "Instrumental"}, {34 , "Acid"}, {35 , "House"}, {36 , "Game"}, {37 , "Sound Clip"}, {38 , "Gospel"}, {39 , "Noise"}, {40 , "Alternative Rock"},
	{41 , "Bass"}, {42 , "Soul"}, {43 , "Punk"}, {44 , "Space"}, {45 , "Meditative"}, {46 , "Instrumental Pop"}, {47 , "Instrumental Rock"}, {48 , "Ethnic"}, {49 , "Gothic"}, {50 , "Darkwave"},
	{51 , "Techno-Industrial"}, {52 , "Electronic"}, {53 , "Pop-Folk"}, {54 , "Eurodance"}, {55 , "Dream"}, {56 , "Southern Rock"}, {57 , "Comedy"}, {58 , "Cult"}, {59 , "Gangsta"}, {60 , "Top 40"},
	{61 , "Christian Rap"}, {62 , "Pop/Funk"}, {63 , "Jungle"}, {64 , "Native US"}, {65 , "Cabaret"}, {66 , "New Wave"}, {67 , "Psychadelic"}, {68 , "Rave"}, {69 , "Showtunes"}, {70 , "Trailer"},
	{71 , "Lo-Fi"}, {72 , "Tribal"}, {73 , "Acid Punk"}, {74 , "Acid Jazz"}, {75 , "Polka"}, {76 , "Retro"}, {77 , "Musical"}, {78 , "Rock & Roll"}, {79 , "Hard Rock"}, {80 , "Folk"},
	{81 , "Folk-Rock"}, {82 , "National Folk"}, {83 , "Swing"}, {84 , "Fast Fusion"}, {85 , "Bebob"}, {86 , "Latin"}, {87 , "Revival"}, {88 , "Celtic"}, {89 , "Bluegrass"}, {90 , "Avantgarde"},
	{91 , "Gothic Rock"}, {92 , "Progressive Rock"}, {93 , "Psychedelic Rock"}, {94 , "Symphonic Rock"}, {95 , "Slow Rock"}, {96 , "Big Band"}, {97 , "Chorus"}, {98 , "Easy Listening"}, {99 , "Acoustic"},
	{100 , "Humour"}, {101 , "Speech"}, {102 , "Chanson"}, {103 , "Opera"}, {104 , "Chamber Music"}, {105 , "Sonata"}, {106 , "Symphony"}, {107 , "Booty Bass"}, {108 , "Primus"}, {109 , "Porn Groove"},
	{110 , "Satire"}, {111 , "Slow Jam"}, {112 , "Club"}, {113 , "Tango"}, {114 , "Samba"}, {115 , "Folklore"}, {116 , "Ballad"}, {117 , "Power Ballad"}, {118 , "Rhytmic Soul"}, {119 , "Freestyle"}, {120 , "Duet"},
	{121 , "Punk Rock"}, {122 , "Drum Solo"}, {123 , "A capella"}, {124 , "Euro-House"}, {125 , "Dance Hall"}, {126 , "Goa"}, {127 , "Drum & Bass"}, {128 , "Club-House"}, {129 , "Hardcore"}, {130 , "Terror"},
	{131 , "Indie"}, {132 , "BritPop"}, {133 , "Negerpunk"}, {134 , "Polsk Punk"}, {135 , "Beat"}, {136 , "Christian Gangsta"}, {137 , "Heavy Metal"}, {138 , "Black Metal"}, {139 , "Crossover"}, {140 , "Contemporary C"},
	{141 , "Christian Rock"}, {142 , "Merengue"}, {143 , "Salsa"}, {144 , "Thrash Metal"}, {145 , "Anime"}, {146 , "JPop"}, {147 , "SynthPop"}
};

void id3debug(struct id3tag* node)
{
	if(node == NULL) return;

	debug(278, "Version: %d", node->version);
	debug(278, "Title: %s", node->title);
	debug(278, "Artist: %s", node->artist);
	debug(278, "Album: %s", node->album);
	debug(278, "Year: %s", node->year);
	debug(278, "Comment: %s", node->comment);
	debug(278, "Genrecode: %s", node->genrecode);
	debug(278, "Genretext: %s", node->genretext);
	debug(278, "Track: %d", node->track);
	debug(278, "Len: %d", node->len);
	debug(278, "Picturetype: %d", node->picturetype);
	debug(278, "Picturepos: %d", node->picturepos);
	debug(278, "Picturelen: %d", node->picturelen);
	debug(278, "Poster: %s", node->poster);
}

void id3writepic(int fd, struct id3tag* node, char* outfile)
{
	int fdw = -1;
	char* buf = NULL;

	if(fd < 0 || node == NULL || node->picturepos == 0 || node->picturelen == 0) return;

	buf = malloc(node->picturelen);
	if(buf == NULL)
	{
		err("no mem");
		return;
	}

	if(node->picturetype == 0) //jpg
	{
		if(outfile == NULL) outfile = TMPID3PICJPG;
		fdw = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	}
	else if(node->picturetype == 1) //png
	{
		if(outfile == NULL) outfile = TMPID3PICPNG;
		fdw = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	}

	if(fdw < 0)
	{
		free(buf);
		perr("write id3 picture file");
		return;
	}

	lseek(fd, node->picturepos, SEEK_SET);
	read(fd, buf, node->picturelen);
	write(fdw, buf, node->picturelen);
	
	node->poster = ostrcat(outfile, NULL, 0, 0); 

	free(buf);
	close(fdw);
}

void freeid3(struct id3tag* node)
{
	if(node == NULL) return;

	free(node->title); node->title = NULL;
	free(node->artist); node->artist = NULL;
	free(node->album); node->album = NULL;
	free(node->year); node->year = NULL;
	free(node->comment); node->comment = NULL;
	free(node->genrecode); node->genrecode = NULL;
	free(node->genretext); node->genretext = NULL;
	free(node->poster); node->poster = NULL;
	
	free(node); node = NULL;
	
	unlink(TMPID3PICJPG);
	unlink(TMPID3PICPNG);
}

int id3swapendian16(int value)
{
	int i = value;

	i = ((i & 0xFF00) >> 8) | ((i & 0x00FF) << 8);
	return i;
}

int id3swapendian32(int value)
{
	int i = value;

	i = ((i & 0xFF000000) >> 24) | ((i & 0x00FF0000) >> 8) | ((i & 0x0000FF00) << 8) | ((i & 0x000000FF) << 24);
	return i;
}

//Search for FF+D8+FF bytes (first bytes of a jpeg image)
//Returns file position:
int id3jpgstart(int fp, int delta)
{
	int ret = -1;
	int i = 0;
	unsigned char tmpstr[3];

	int startpos = lseek(fp, 0, SEEK_CUR);
	
	for(i = 0; i < delta; i++)
	{
		read(fp, tmpstr, sizeof(tmpstr));
		if(tmpstr[0] == 0xFF && tmpstr[1] == 0xD8 && tmpstr[2] == 0xFF)
		{
			ret = lseek(fp, 0, SEEK_CUR) - 3;
			break;
		}
		lseek(fp, -2, SEEK_CUR);
	}
	
	lseek(fp, startpos, SEEK_SET);
	return ret;
}

//Search for 89 50 4E 47 0D 0A 1A 0A 00 00 00 0D 49 48 44 52 bytes (first bytes of a PNG image)
//Returns file position:
int id3pngstart(int fp, int delta)
{
	int ret = -1, treffer = 0;
	int i = 0, j = 0;
	unsigned char tmpstr[16];
	unsigned char pngchar[16] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52};

	int startpos = lseek(fp, 0, SEEK_CUR);

	for(i = 0; i < delta; i++)
	{
		read(fp, tmpstr, sizeof(tmpstr));
		treffer = 1;
		for(j = 0; j < 16; j++)
		{
			if(tmpstr[j] != pngchar[j])
			{
				treffer = 0;
				break;
			}
		}

		if(treffer == 1)
		{
			ret = lseek(fp, 0, SEEK_CUR) - 16;
			break;
		}
		
		lseek(fp, -15, SEEK_CUR);
	}
	
	lseek(fp, startpos, SEEK_SET);
	return ret;
}

char* id3readtag(int fd, int taglen, int maxtaglen)
{
	int i = 0, y = 0;
	char* tmpstr = NULL;

	if(taglen > maxtaglen) taglen = maxtaglen;

	tmpstr = malloc(taglen + 1);
	if(tmpstr == NULL) return NULL; 

	read(fd, tmpstr, taglen);
	tmpstr[taglen] = '\0';

	//remove non printable chars
	for(i = 0; i < taglen; i++)
	{
		if((tmpstr[i] >= 0x20 && tmpstr[i] < 0x7F) || (tmpstr[i] >= 0xC0 && tmpstr[i] < 0xFD))
		{
			tmpstr[y] = tmpstr[i];
			y++;
		}
	}
	tmpstr[y] = '\0';

	return tmpstr;
}

int id3tagsize(int fd)
{
	int size = 0;

	if(fd < 0) return 0;

	lseek(fd, 6, SEEK_SET);
	read(fd, &size, sizeof(unsigned int));

	//The ID3 tag size is encoded with four bytes where the first bit
	//(bit 7) is set to zero in every byte, making a total of 28 bits. The zeroed
	//bits are ignored, so a 257 bytes long tag is represented as $00 00 02 01.

	size = id3swapendian32(size);
	size = (((size & 0x7f000000) >> 3 ) | ((size & 0x7f0000) >> 2 ) | ((size & 0x7f00) >> 1 ) | (size & 0x7f));

	lseek(fd, 0, SEEK_SET);
	return size;
}

int id3getversion(int fd)
{
	char sig[3];
	unsigned short int version = 0;

	if(fd < 0) return -1;

	read(fd, sig, sizeof(sig));
	if(ostrncmp("ID3", sig, 3) == 0)
		read(fd, &version, sizeof(unsigned short int));

	version = id3swapendian16(version);
	version /= 256;

	lseek(fd, 0, SEEK_SET);
	return (int)version;
}

void id3parse1_0(int fd, struct id3tag *node)
{
	char buf[128];

	if(fd < 0 || node == NULL) return;

	lseek(fd, -128, SEEK_END);
	read(fd, buf, 128);

	if(strstr(buf, "TAG") != NULL)
	{
		lseek(fd, -125, SEEK_END);
		if(node->title == NULL) node->title = malloc(31);
		if(node->title != NULL)
		{
			read(fd, node->title, 30);
			node->title[30] = '\0';
		}

		lseek(fd, -95, SEEK_END);
		if(node->artist == NULL) node->artist = malloc(31);
		if(node->artist != NULL)
		{
			read(fd, node->artist, 30);
			node->artist[30] = '\0';
		}

		lseek(fd, -65, SEEK_END);
		if(node->album == NULL) node->album = malloc(31);
		if(node->album != NULL)
		{
			read(fd, node->album, 30);
			node->album[30] = '\0';
		}

		lseek(fd, -35, SEEK_END);
		if(node->year == NULL) node->year = malloc(5);
		if(node->year != NULL)
		{
			read(fd, node->year, 4);
			node->year[4] = '\0';
		}

		lseek(fd, -31, SEEK_END);
		if(node->comment == NULL) node->comment = malloc(31);
		if(node->comment != NULL)
		{
			read(fd, node->comment, 30);
			node->comment[30] = '\0';
		}

		lseek(fd, -1, SEEK_END);
		if(node->genrecode == NULL) node->genrecode = malloc(2);
		if(node->genrecode != NULL)
		{
			read(fd, node->genrecode, 1);
			node->genrecode[1] = '\0';
		}

		//Track
		if(node->comment != NULL)
		{
			if(*(node->comment + 28) == 0 && *(node->comment + 29) > 0)
			{
				node->track = (int)*(node->comment + 29);
				node->version = 1;
			}
			else
			{
				node->track = 1;
				node->version = 0;
			}
		}

		if(node->genrecode != NULL)
		{
			if((int)node->genrecode[0] >= 0 && (int)node->genrecode[0] < sizeof(id3genrelist) / sizeof(struct id3genre))
			{
				free(node->genretext); node->genretext = NULL;
				//node->genretext = ostrcat(id3genrelist[(int)node->genrecode[0]].text, NULL, 0, 0);
				node->genretext = malloc(31);
				strcpy(node->genretext, id3genrelist[(int)node->genrecode[0]].text);
				node->genretext[30] = '\0';
			}
		}
	}
}

void id3parse2_2(int fd, struct id3tag *node)
{
	int size = 0;
	int taglen = 0;
	char tag[3];
	char* buf = NULL;
	
	if(fd < 0 || node == NULL) return;

	size = id3tagsize(fd);
	lseek(fd, 10, SEEK_SET);

	while(size != 0)
	{
		read(fd, tag, 3);
		size -= 3;

		//read 3 byte big endian tag length
		read(fd, &taglen, sizeof(unsigned int));
		lseek(fd, -1, SEEK_CUR);

		taglen = id3swapendian32(taglen);
		taglen = (taglen / 256);
		size -= 3;

		//perform checks for end of tags and tag length overflow or zero
		if(*tag == 0 || taglen > size || taglen == 0) break;

		if(ostrncmp("TP1", tag, 3) == 0) //artist
		{
			lseek(fd, 1, SEEK_CUR);
			free(node->artist); node->artist = NULL;
			node->artist = id3readtag(fd, taglen - 1, 260);
		}
		else if(ostrncmp("TP2", tag, 3) == 0) //title
		{
			lseek(fd, 1, SEEK_CUR);
			free(node->title); node->title = NULL;
			node->title = id3readtag(fd, taglen - 1, 260);
		}
		else if(ostrncmp("TAL", tag, 3) == 0) //album
		{
			lseek(fd, 1, SEEK_CUR);
			free(node->album); node->album = NULL;
			node->album = id3readtag(fd, taglen - 1, 260);
		}
		else if(ostrncmp("TRK", tag, 3) == 0) //track nr.
		{
			lseek(fd, 1, SEEK_CUR);
			buf = id3readtag(fd, taglen - 1, 8);
			if(buf != NULL) node->track = atoi(buf);
			free(buf); buf = NULL;
		}
		else if(ostrncmp("TYE", tag, 3) == 0) //year
		{
			lseek(fd, 1, SEEK_CUR);
			free(node->year); node->year = NULL;
			node->year = id3readtag(fd, taglen - 1, 12);
		}
		else if(ostrncmp("TLE", tag, 3) == 0) //length
		{
			lseek(fd, 1, SEEK_CUR);
			buf = id3readtag(fd, taglen - 1, 264);
			if(buf != NULL) node->len = atoi(buf);
			free(buf); buf = NULL;
		}
		else if(ostrncmp("COM", tag, 3) == 0) //comment
		{
			lseek(fd, 1, SEEK_CUR);
			free(node->comment); node->comment = NULL;
			node->comment = id3readtag(fd, taglen - 1, 260);
		}
		else if(ostrncmp("TCO", tag, 3) == 0) //genre
		{
			lseek(fd, 1, SEEK_CUR);
			free(node->genretext); node->genretext = NULL;
			node->genretext = id3readtag(fd, taglen - 1, 260);
		}
		else if(ostrncmp("PIC", tag, 3) == 0) //picture
		{
			lseek(fd, 1, SEEK_CUR);
			lseek(fd, 5, SEEK_CUR);
			node->picturetype = 0; //jpg
			node->picturepos = id3jpgstart(fd, 20);
			
			if(node->picturepos < 0)
			{
				node->picturetype = 1; //png
				node->picturepos = id3pngstart(fd, 20);
			}
			
			taglen = taglen - (node->picturepos - lseek(fd, 0, SEEK_CUR));
			node->picturelen = taglen - 6;
			lseek(fd, taglen - 6, SEEK_CUR);
			
			if(node->picturepos < 0)
			{
				node->picturetype = 0;
				node->picturepos = 0;
				node->picturelen = 0;
			}
		}
		else
		{
			lseek(fd, taglen, SEEK_CUR);
		}
		size -= taglen;
	}

	node->version = 2;
}

void id3parse2_3(int fd, struct id3tag *node)
{
	int size = 0;
	int taglen = 0;
	char tag[4];
	char* buf = NULL;
	
	if(fd < 0 || node == NULL) return;

	size = id3tagsize(fd);
	lseek(fd, 10, SEEK_SET);

	while(size != 0)
	{
		read(fd, tag, 4);
		size -= 4;

		//read 4 byte big endian tag length
		read(fd, &taglen, sizeof(unsigned int));
		taglen = id3swapendian32(taglen);
		size -= 4;

		lseek(fd, 2, SEEK_CUR);
		size -= 2;

		//perform checks for end of tags and tag length overflow or zero
		if(*tag == 0 || taglen > size || taglen == 0) break;

		if(ostrncmp("TPE1", tag, 4) == 0) //artist
		{
			lseek(fd, 1, SEEK_CUR);
			free(node->artist); node->artist = NULL;
			node->artist = id3readtag(fd, taglen - 1, 260);
		}
		else if(ostrncmp("TIT2", tag, 4) == 0) //title
		{
			lseek(fd, 1, SEEK_CUR);
			free(node->title); node->title = NULL;
			node->title = id3readtag(fd, taglen - 1, 260);
		}
		else if(ostrncmp("TALB", tag, 4) == 0) //album
		{
			lseek(fd, 1, SEEK_CUR);
			free(node->album); node->album = NULL;
			node->album = id3readtag(fd, taglen - 1, 260);
		}
		else if(ostrncmp("TRCK", tag, 4) == 0) //track nr
		{
			lseek(fd, 1, SEEK_CUR);
			buf = id3readtag(fd, taglen - 1, 8);
			if(buf != NULL) node->track = atoi(buf);
			free(buf); buf = NULL;
		}
		else if(ostrncmp("TYER", tag, 4) == 0) //year
		{
			lseek(fd, 1, SEEK_CUR);
			free(node->year); node->year = NULL;
			node->year = id3readtag(fd, taglen - 1, 12);
		}
		else if(ostrncmp("TLEN", tag, 4) == 0) //length in milliseconds
		{
			lseek(fd, 1, SEEK_CUR);
			buf = id3readtag(fd, taglen - 1, 264);
			if(buf != NULL) node->len = atol(buf) / 1000;
			free(buf); buf = NULL;
		}
		else if(ostrncmp("TCON", tag, 4) == 0) //genre
		{
			lseek(fd, 1, SEEK_CUR);
			free(node->genretext); node->genretext = NULL;
			node->genretext = id3readtag(fd, taglen - 1, 260);
		}
		else if(ostrncmp("COMM", tag, 4) == 0) //comment
		{
			lseek(fd, 1, SEEK_CUR);
			free(node->comment); node->comment = NULL;
			node->comment = id3readtag(fd, taglen - 1, 260);
		}
		else if(ostrncmp("APIC", tag, 4) == 0) //picture
		{
			lseek(fd, 1, SEEK_CUR);
			lseek(fd, 12, SEEK_CUR);
			node->picturetype = 0; //jpg
			node->picturepos = id3jpgstart(fd, 20);
			
			if(node->picturepos < 0)
			{
				node->picturetype = 1; //png
				node->picturepos = id3pngstart(fd, 20);
			}
			
			taglen = taglen - (node->picturepos - lseek(fd, 0, SEEK_CUR));
			node->picturelen = taglen - 13;
			lseek(fd, taglen - 13, SEEK_CUR);
			
			if(node->picturepos < 0)
			{
				node->picturetype = 0;
				node->picturepos = 0;
				node->picturelen = 0;
			}
		}
		else
		{
			lseek(fd, taglen, SEEK_CUR);
		}
		size -= taglen;
	}
	
	node->version = 3;
}


void id3parse2_4(int fd, struct id3tag *node)
{
	int size = 0;
	int taglen = 0;
	char tag[4];
	char* buf = NULL;

	if(fd < 0 || node == NULL) return;
	
	size = id3tagsize(fd);
	lseek(fd, 10, SEEK_SET);

	while(size != 0)
	{
		read(fd, tag, 4);
		size -= 4;

		//read 4 byte big endian tag length 
		read(fd, &taglen, sizeof(unsigned int));
		taglen = id3swapendian32(taglen);
		size -= 4;

		lseek(fd, 2, SEEK_CUR);
		size -= 2;

		//perform checks for end of tags and tag length overflow or zero 
		if(*tag == 0 || taglen > size || taglen == 0) break;

		if(ostrncmp("TPE1", tag, 4) == 0) //artist
		{
			lseek(fd, 1, SEEK_CUR);
			free(node->artist); node->artist = NULL;
			node->artist = id3readtag(fd, taglen - 1, 260);
		}
		else if(ostrncmp("TIT2", tag, 4) == 0) //title
		{
			lseek(fd, 1, SEEK_CUR);
			free(node->title); node->title = NULL;
			node->title = id3readtag(fd, taglen - 1, 260);
		}
		else if(ostrncmp("TALB", tag, 4) == 0) //album
		{
			lseek(fd, 1, SEEK_CUR);
			free(node->album); node->album = NULL;
			node->album = id3readtag(fd, taglen - 1, 260);
		}
		else if(ostrncmp("TRCK", tag, 4) == 0) //track nr
		{
			lseek(fd, 1, SEEK_CUR);
			buf = id3readtag(fd, taglen - 1, 8);
			if(buf != NULL) node->track = atoi(buf);
			free(buf); buf = NULL;
		}
		else if(ostrncmp("TYER", tag, 4) == 0) //year
		{
			lseek(fd, 1, SEEK_CUR);
			free(node->year); node->year = NULL;
			node->year = id3readtag(fd, taglen - 1, 12);
		}
		else if(ostrncmp("TLEN", tag, 4) == 0) //length in milliseconds
		{
			lseek(fd, 1, SEEK_CUR);
			buf = id3readtag(fd, taglen - 1, 264);
			if(buf != NULL) node->len = atol(buf) / 1000;
			free(buf); buf = NULL;
		}
		else if(ostrncmp("TCON", tag, 4) == 0) //genre
		{
			lseek(fd, 1, SEEK_CUR);
			free(node->genretext); node->genretext = NULL;
			node->genretext = id3readtag(fd, taglen - 1, 260);
		}
		else if(ostrncmp("COMM", tag, 4) == 0) //comment
		{
			lseek(fd, 1, SEEK_CUR);
			free(node->comment); node->comment = NULL;
			node->comment = id3readtag(fd, taglen - 1, 260);
		}
		else if(ostrncmp("APIC", tag, 4) == 0) //picture
		{
			lseek(fd, 1, SEEK_CUR);
			lseek(fd, 12, SEEK_CUR);
			node->picturetype = 0; //jpg
			node->picturepos = id3jpgstart(fd, 20);
			
			if(node->picturepos < 0)
			{
				node->picturetype = 1; //png
				node->picturepos = id3pngstart(fd, 20);
			}
			
			taglen = taglen - (node->picturepos - lseek(fd, 0, SEEK_CUR));
			node->picturelen = taglen - 13;
			lseek(fd, taglen - 13, SEEK_CUR);
			
			if(node->picturepos < 0)
			{
				node->picturetype = 0;
				node->picturepos = 0;
				node->picturelen = 0;
			}
		}
		else
		{
			lseek(fd, taglen, SEEK_CUR);
		}
		size -= taglen;
	}
	
	node->version = 4;
}

int id3parse(int fd, struct id3tag *node)
{
	switch(id3getversion(fd))
	{
		case 0:
			id3parse1_0(fd, node);
			break;
		case 2:
			id3parse2_2(fd, node);
			break;
		case 3:
			id3parse2_3(fd, node);
			break;
		case 4:
			id3parse2_4(fd, node);
			break;
		default:
			return -1;
	}
	
	return 0;
}

//flag: 0 = save in tmp
//flag: 1 = save pic in mediadb path if pic not exist
//flag: 2 = save no pic
struct id3tag* getid3(char* file, char* id, int flag)
{
	int fd = -1;
	struct id3tag *node = NULL;
	char* savefile = NULL, * savethumb = NULL;
	unsigned char* buf = NULL;
	int channels = 0;
	unsigned long width = 0, height = 0, rowbytes = 0;
	
	if(file == NULL) return NULL;
	
	fd = open(file, O_RDONLY);
	if(fd < 0) return NULL;

	node = (struct id3tag*)calloc(1, sizeof(struct id3tag));
	if(node != NULL)
	{
		id3parse(fd, node);

		//del spaces
		node->title = strstrip(node->title);
		node->artist = strstrip(node->artist);
		node->album = strstrip(node->album);
		node->year = strstrip(node->year);
		node->comment = strstrip(node->comment);
		node->genrecode = strstrip(node->genrecode);
		node->genretext = strstrip(node->genretext);
		node->poster = strstrip(node->poster);

		if(flag == 1 && id != NULL)
		{
			savefile = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
			savefile = ostrcat(savefile, id, 1, 0);
			savefile = ostrcat(savefile, "_cover.jpg", 1, 0);
			
			if(!file_exist(savefile))
				id3writepic(fd, node, savefile);

			//create thumb
			savethumb = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
			savethumb = ostrcat(savethumb, id, 1, 0);
			savethumb = ostrcat(savethumb, "_thumb.jpg", 1, 0);
			if(file_exist(savefile) && !file_exist(savethumb))
			{
				buf = loadjpg(savefile, &width, &height, &rowbytes, &channels, 16);
				buf = savejpg(savethumb, width, height, channels, 91, 140, 70, buf);
			}
			free(savethumb); savethumb = NULL;
			free(buf); buf = NULL;

			free(savefile); savefile = NULL;
		}
		else if(flag == 0)
			id3writepic(fd, node, NULL);	
	}
	
	close(fd);
	return node;
}

#endif

#! /bin/sh
rm cache.beeg.*
rm Beeg.*.m3u
rm -rf _single
rm -rf _full/beeg
mkdir -p _full/beeg/streams
mkdir _single
wget http://beeg.com -O cache.beeg.main.html
sections=`cat cache.beeg.main.html | grep '^    <li><a target="_self" href="http://beeg.com/tag'  | tr '"' '\n' | grep ^http: | sort -u`

cat cache.beeg.main.html | grep http://beeg.com | grep -v http://beeg.com/tag | grep alt= | sed 's/<a href="/link=/' | sed 's/"><img src="/#pic=/' | sed 's/" title="/"\n/' | sed 's/" alt="/#title="/' | grep ^link= | tr ' ' '|'>> cache.beeg.list

piccount=0
count=0
for ROUND in $sections; do	
	count=`expr $count + 1`
#	echo "sections($count)=$ROUND"
	wget $ROUND -O cache.beeg.section."$count".html
	cat cache.beeg.section."$count".html | grep http://beeg.com | grep -v http://beeg.com/tag | grep alt= | sed 's/<a href="/link=/' | sed 's/"><img src="/#pic=/' | sed 's/" title="/"\n/' | sed 's/" alt="/#title="/' | grep ^link= | tr ' ' '|'>> cache.beeg.list
#	if [ $count = 10 ];then
#		break
#	fi
done
LIST=`cat cache.beeg.list`
echo "# Beeg.com" > cache.beeg.playlist

count=0
for ROUND in $LIST; do
	URL=`echo $ROUND | tr '#' '\n' | grep ^link= | sed 's/link=//' | tr '|' ' '`
	PIC=`echo $ROUND | tr '#' '\n' | grep ^pic= | sed 's/pic=//' | tr '|' ' '`
	TITLE=`echo $ROUND | tr '#' '\n' | grep ^title= | sed 's/title=//' | tr '|' ' ' | tr -d '"'`
	CHECK=`echo $URL | cut -d / -f4`
	if [ `cat cache.beeg.titanlist | grep "$CHECK" | wc -l` -eq 0 ];then
		count=`expr $count + 1`
		echo "add($count)=$ROUND"
		wget $URL -O cache.beeg.stream."$count".html
		STREAMURL=`cat cache.beeg.stream."$count".html | grep "'file':" | cut -d "'" -f4`
		piccount=`expr $piccount + 1`
		LINE="$TITLE#$STREAMURL#$PIC#beeg_$piccount.jpg#Beeg#2"
		echo $LINE >> cache.beeg.titanlist
		echo $STREAMURL >> cache.beeg.playlist
		echo "$STREAMURL" >> _single/Beeg.`echo "$TITLE" | tr '_' '.' | tr ' ' '.' | tr '...' '.' | tr '..' '.' | tr '..' '.' | tr '..' '.'`."$count".m3u
#		if [ $count = 10 ];then
#			break
#		fi
	fi
done

################
cp cache.beeg.titanlist _full/beeg/streams/beeg.all-newfirst.list
cat cache.beeg.titanlist | sort -u > _full/beeg/streams/beeg.all-sorted.list

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	if [ `cat cache.beeg.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.beeg.titanlist | grep ^"$ROUND" > cache.beeg.titanlist."$ROUND"
		cat cache.beeg.titanlist."$ROUND" | sort -u > _full/beeg/streams/beeg.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/beeg/streams/beeg."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Beeg#1 >> _full/beeg/beeg.a-z.list
	fi
done

SECTION=`cat cache.beeg.main.html | grep "http://beeg.com/tag/" | sed 's/    <li><a target="_self" href="/link=/' | sed 's/">/#title=/' | sed 's/" >/#title=/' | sed 's!</a></li>!!' | tr ' ' '|'`
echo Section $SECTION

for ROUND in $SECTION; do
	echo Section $ROUND

	URL=`echo $ROUND | tr '#' '\n' | grep ^link= | sed 's/link=//' | tr '|' ' '`
	TITLE=`echo $ROUND | tr '#' '\n' | grep ^title= | sed 's/title=//' | tr '|' '-' | tr -d '"'`	
	echo "$TITLE""#http://atemio.dyndns.tv/mediathek/beeg/streams/beeg."`echo "$TITLE" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$TITLE" | tr 'A-Z' 'a-z'`.jpg#`echo "$TITLE" | tr 'A-Z' 'a-z'`.jpg"#Beeg#1 >> _full/beeg/beeg.category.list

	rm cache.beeg.main.html
	wget $URL -O cache.beeg.section.`echo "$TITLE" | tr 'A-Z' 'a-z'`.html
	file=cache.beeg.section.`echo "$TITLE" | tr 'A-Z' 'a-z'`.html

#	sections=`cat $file | grep '^<a href="http://beeg.com/section' | tr '"' '\n' | grep ^http:`
	sections=`cat $file | grep '^    <li><a target="_self" href="http://beeg.com/tag'  | tr '"' '\n' | grep ^http: | sort -u`

	cat cache.beeg.section.`echo "$TITLE" | tr 'A-Z' 'a-z'`.html | grep http://beeg.com | grep -v http://beeg.com/tag | grep alt= | sed 's/<a href="/link=/' | sed 's/"><img src="/#pic=/' | sed 's/" title="/"\n/' | sed 's/" alt="/#title="/' | grep ^link= | tr ' ' '|'> cache.beeg.section.list

	count=0
	for ROUND in $sections; do
		count=`expr $count + 1`
		wget $ROUND -O cache.beeg.section.`echo "$TITLE" | tr 'A-Z' 'a-z'`."$count".html
		cat cache.beeg.section.`echo "$TITLE" | tr 'A-Z' 'a-z'`."$count".html | grep http://beeg.com | grep -v http://beeg.com/tag | grep alt= | sed 's/<a href="/link=/' | sed 's/"><img src="/#pic=/' | sed 's/" title="/"\n/' | sed 's/" alt="/#title="/' | grep ^link= | tr ' ' '|'>> cache.beeg.section.list
	done
	
	LIST=`cat cache.beeg.section.list`
	count=0
	for ROUND in $LIST; do
		count=`expr $count + 1`
		CHECK=`echo $ROUND | cut -d"#" -f1 | sed 's!link=http://beeg.com/!!'`
		if [ `cat cache.beeg.titanlist | grep "$CHECK" | wc -l` -eq 1 ];then
			echo "add($count)=$ROUND"
#			cat cache.beeg.titanlist | grep "$CHECK" >> _full/beeg/streams/beeg.`echo "$TITLE" | tr 'A-Z' 'a-z'`.list
			cat cache.beeg.titanlist | grep "$CHECK" >> cache.beeg.section.last.`echo "$TITLE" | tr 'A-Z' 'a-z'`.list
		fi
	done
	cat cache.beeg.section.last.`echo "$TITLE" | tr 'A-Z' 'a-z'`.list | sort -u > _full/beeg/streams/beeg.`echo "$TITLE" | tr 'A-Z' 'a-z'`.list	
done

################
rm cache.beeg.*
rm Beeg.*.m3u
exit
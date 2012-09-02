#! /bin/sh
echo beeg started
date
rm cache.beeg.*
rm Beeg.*.m3u
rm -rf _single
rm -rf _full/beeg
mkdir -p _full/beeg/streams
mkdir _single
wget http://beeg.com -O cache.beeg.main.html

sections=`cat cache.beeg.main.html | grep ^'<a href="http://beeg.com/section/home/' | cut -d '"' -f2`
cat cache.beeg.main.html | grep http://beeg.com | grep -v http://beeg.com/tag | grep alt= | sed 's/<a href="/link=/' | sed 's/"><img src="/#pic=/' | sed 's/" title="/"\n/' | sed 's/" alt="/#title="/' | grep ^link= | tr ' ' '|'>> cache.beeg.list

piccount=0
count=0
for ROUND in $sections; do	
	count=`expr $count + 1`
	echo "sections($count)=$ROUND"

	wget $ROUND -O cache.beeg.section."$count".html
	cat cache.beeg.section."$count".html | grep http://beeg.com | grep -v http://beeg.com/tag | grep alt= | sed 's/<a href="/link=/' | sed 's/"><img src="/#pic=/' | sed 's/" title="/"\n/' | sed 's/" alt="/#title="/' | grep ^link= | tr ' ' '|'>> cache.beeg.list
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
	fi
done

cp cache.beeg.titanlist _full/beeg/streams/beeg.all-newfirst.list
cat cache.beeg.titanlist | sort -u > _full/beeg/streams/beeg.all-sorted.list

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	if [ `cat cache.beeg.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.beeg.titanlist | grep ^"$ROUND" > cache.beeg.titanlist."$ROUND"
		cat cache.beeg.titanlist."$ROUND" | sort -u > _full/beeg/streams/beeg.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/beeg/streams/beeg."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Beeg#1 >> _full/beeg/beeg.a-z.list
	fi
done

SECTION=`cat cache.beeg.main.html | grep "http://beeg.com/tag/" | sed 's/    <li><a target="_self" href="/link=/' | sed 's/">/#title=/' | sed 's/" >/#title=/' | sed 's!</a></li>!!' | tr ' ' '|' | grep "^link="`
echo Section $SECTION

for ROUND in $SECTION; do
	echo Section $ROUND

ncount=`expr $ncount + 1`
	URL=`echo $ROUND | tr '#' '\n' | grep ^link= | sed 's/link=//' | tr '|' ' '`
	TITLE=`echo $ROUND | tr '#' '\n' | grep ^title= | sed 's/title=//' | tr '|' '-' | tr -d '"'`	
	echo "$TITLE""#http://atemio.dyndns.tv/mediathek/beeg/streams/beeg."`echo "$TITLE" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$TITLE" | tr 'A-Z' 'a-z'`.jpg#`echo "$TITLE" | tr 'A-Z' 'a-z'`.jpg"#Beeg#1 >> _full/beeg/beeg.category.list
	TAG=`echo $ROUND | tr '#' '\n' | grep ^link= | sed 's/link=//' | tr '|' ' ' | cut -d "/" -f5`	

	rm cache.beeg.main.html
	wget $URL -O cache.beeg.section.`echo "$TITLE" | tr 'A-Z' 'a-z'`.html
	file=cache.beeg.section.`echo "$TITLE" | tr 'A-Z' 'a-z'`.html
	echo $file

	sectionfiles=`cat $file | grep -v "/tag/" | grep '^<a href="http://beeg.com/' | cut -d "/" -f4 | cut -d '"' -f1`
	echo sectionfiles $sectionfiles

	for ROUND1 in $sectionfiles; do
		if [ `cat cache.beeg.titanlist | grep "$ROUND1" | wc -l` -gt 0 ];then
			echo `cat cache.beeg.titanlist | grep "/$ROUND1."` >> cache.beeg.section.`echo "$TITLE" | tr 'A-Z' 'a-z'`.list
			cat cache.beeg.section.`echo "$TITLE" | tr 'A-Z' 'a-z'`.list | sort -u > _full/beeg/streams/beeg.`echo "$TAG" | tr 'A-Z' 'a-z'`.list
		fi
	done

	file1=`echo "$TITLE" | tr 'A-Z' 'a-z'`
	ls cache.beeg.section.$file1.html
	secondlist=`cat cache.beeg.section.$file1.html | grep "^<a href=\"http://beeg.com/tag/$TAG" | cut -d '"' -f2`
	echo secondlist $secondlist

	for ROUND2 in $secondlist; do
		count1=`echo $ROUND2 | cut -d '/' -f6`
		echo count1 $count1
		
		URL=$ROUND2
		wget $URL -O cache.beeg.section.`echo "$TITLE" | tr 'A-Z' 'a-z'`.$count1.html

		file=cache.beeg.section.`echo "$TITLE" | tr 'A-Z' 'a-z'`.$count1.html
		echo $file
	
		sectionfiles=`cat $file | grep -v "/tag/" | grep '^<a href="http://beeg.com/' | cut -d "/" -f4 | cut -d '"' -f1`
		echo sectionfiles $sectionfiles
	
		for ROUND3 in $sectionfiles; do
			if [ `cat cache.beeg.titanlist | grep "$ROUND3" | wc -l` -gt 0 ];then
				echo `cat cache.beeg.titanlist | grep "/$ROUND3."` >> cache.beeg.section.`echo "$TITLE" | tr 'A-Z' 'a-z'`.$count1.list
				cat cache.beeg.section.`echo "$TITLE" | tr 'A-Z' 'a-z'`.$count1.list | sort -u > _full/beeg/streams/beeg.`echo "$TAG" | tr 'A-Z' 'a-z'`.list
			fi
		done
	done
done

category_file=`cat _full/beeg/beeg.category.list | cut -d "#" -f2 | sed 's!http://atemio.dyndns.tv/mediathek/beeg/streams/!!'`
category_files=`ls -1 _full/beeg/streams`
count=0
for ROUND1 in $category_file; do
	for ROUND2 in $category_files; do
#		echo "list ($count) $ROUND1 $ROUND2"
		if [ "$ROUND1" == "$ROUND2" ]; then
			LINE=`cat _full/beeg/beeg.category.list | grep "$ROUND1"`
			if [ ! -z "$LINE" ];then
				count=`expr $count + 1`
				echo "$LINE" >> _full/beeg/beeg.category.list.filter
				echo "add ($count) $ROUND1 $ROUND2"
			fi
		fi
	done
done

in=`cat _full/beeg/beeg.category.list | wc -l`
out=`cat _full/beeg/beeg.category.list.filter | wc -l`

echo "beeg.category.list        $in"
echo "beeg.category.list.filter $out"
	
if [ "$in" -gt "$out" ];then
	cat _full/beeg/beeg.category.list.filter | sort -um > _full/beeg/beeg.category.sorted.list
	mv -f _full/beeg/beeg.category.list _full/beeg/beeg.category.list.searchlist
	mv -f _full/beeg/beeg.category.sorted.list _full/beeg/beeg.category.list
	echo ---------------------- change ----------------------------- beeg.category.list
else
	echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! not change !!!!!!!!!!!!!!!!!!!!"
fi
	
	
date
rm cache.beeg.*
rm Beeg.*.m3u
echo beeg closed
exit
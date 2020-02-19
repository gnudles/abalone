echo \#\!/bin/sh >$5
echo wkguirunner -e $1 -i file://$2 -t $3 -c $4 >> $5
chmod a+x $5

trap "rm /tmp/yoo$$; exit 1" 2 3
cat - > /tmp/yoo$$
mv /tmp/yoo$$ $1

#!/bin/bash

# Tests that multiple rpeetitions of version command from separate processes
# do not crash datalackey.

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) datalackey-executable"
    exit 100
fi
DL=$1

cat > _script.sh << EOF
#!/bin/sh
echo '[1,"version"]'
EOF
chmod a+x _script.sh

set -eu

(
for K in 1 2 3 4 5 6 7 8 9 10
do
    echo '['$K',"run","in","JSON","stdin","out","JSON","stdout","program","./_script.sh"]'
    nap
done
) | $DL -m -i stdin JSON -o stdout JSON > /dev/null

rm -f _script.sh

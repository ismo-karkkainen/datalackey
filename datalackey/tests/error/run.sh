#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
DL=$1
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

cat > _script.sh << EOF
#!/bin/sh
while read L
do
    echo \$L
done
EOF
chmod a+x _script.sh

(
echo '[1,"run","in","JSON","stdin","program","./_script.sh"]'
nap
echo '[1,"run","program","./_script.sh"]'
echo '[1,"end-feed",1]'
nap
echo '[2,"run","invalid"]'
echo '[3,"run","env"]'
echo '[4,"run","env",1,1]'
echo '[5,"run","output","str",0]'
echo '[6,"run","env","invalid=name",0]'
echo '[7,"run","env","name",0,"env","name",1]'
echo '[8,"run","in","JSON","stdin","in","JSON","stdin"]'
echo '[9,"run","in","invalid","stdin"]'
echo '[10,"run","in","JSON","invalid"]'
echo '[11,"run","out","invalid","stdin"]'
echo '[12,"run","out","JSON","invalid"]'
echo '[13,"run","notify","invalid"]'
echo '[14,"run","notify","data","program","./_script.sh"]'
echo '[15,"run","change-directory","/invld","program","./_script.sh"]'
echo '[16,"run","output","data","label","output","data",null,"program","./_script.sh"]'
echo '[17,"run","input","label","data","program","./_script.sh"]'
echo '[18,"run","output","data","label","program","./_script.sh"]'
echo '[19,"run","program","./invld"]'
echo '[20,"run","out","JSON","stdout","out","JSON","stdout"]'
echo '[21,"run","out","JSON","stderr","out","JSON","stderr"]'
) | $DL -m -i stdin JSON -o stdout JSON |
sed -e 's/"running",.*]$/"running",pid]/' -e 's/invld",.*]$/invld",err]/' > "$OUT"

cat > "$EXP" << EOF
[1,"run","running",pid]
[1,"run","error","identifier","in-use"]
[1,"end-feed","",1]
set
[1,"run","input","closed"]
[1,"run","exit",0]
end
[1,"run","finished"]
[2,"run","error","argument","unknown","invalid"]
[3,"run","error","env","argument","missing"]
[4,"run","error","env","argument","not-string"]
[5,"run","error","output","argument","not-string-or-null"]
[6,"run","error","env","argument","invalid","invalid=name"]
[7,"run","error","env","argument","duplicate","name"]
[8,"run","error","in","multiple"]
[9,"run","error","in","format","unknown","invalid"]
[10,"run","error","in","unknown","invalid"]
[11,"run","error","out","format","unknown","invalid"]
[12,"run","error","out","unknown","invalid"]
[13,"run","error","notify","unknown","invalid"]
[14,"run","error","notify","no-input"]
[15,"run","error","change-directory","/invld",err]
[16,"run","error","output","duplicate","data"]
[17,"run","error","in","missing"]
[18,"run","error","out","missing"]
subset
[19,"run","error","program","./invld",err]
[19,"run","error","program","./invld"]
end
[20,"run","error","out","duplicate","stdout"]
[21,"run","error","out","duplicate","stderr"]
EOF

compare-output "$OUT" "$EXP" && rm -f "$OUT" "$EXP" _script.sh

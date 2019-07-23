#!/bin/sh

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
DL=$1
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

cat > _script.sh << EOF
#!/usr/bin/env ruby

require 'json'

raw = \$stdin.read
begin
  data = JSON.parse(raw)
rescue JSON::ParserError => e
  \$stderr.puts e.to_s
  \$stderr.puts raw
  exit 1
end
\$stderr.puts JSON.generate(ARGV)
dada = {}
data.each_pair { |k, v| dada[k + '-out'] = v }
puts JSON.generate(dada)
exit 0
EOF
chmod a+x _script.sh

(
echo '{"label":123}'
echo '["1","run","in","JSON","stdin","out","JSON","stdout","input","label","foo","output-prefix","fed-","end-feed","program","./_script.sh"]'
) | $DL -m -i stdin JSON -o stdout JSON |
replace-pid > $OUT

cat > $EXP <<EOF
[null,"data","stored","label",1]
["1","run","running","pid"]
[null,"process","started","1","pid"]
["1","run","input","closed"]
set
["1","run","exit",0]
[null,"data","stored","fed-foo-out",2]
end
[null,"process","ended","1","pid"]
["1","run","finished"]
["1","done",""]
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP _script.sh

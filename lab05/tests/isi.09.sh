cat isi.s tests/m9.s > exe.$$.s
trap "rm -f exe.$$.s" EXIT
1521 spim -file exe.$$.s | sed 1d

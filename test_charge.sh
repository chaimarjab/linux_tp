#!/bin/bash
# Usage : ./test_charge.sh NB_CLIENTS
N=${1:-5}
D=$(mktemp -d)

# Lancement simultané (timeout : un client bloqué est détecté au bout de 10 s)
for i in $(seq 1 $N); do
  ( timeout 10 ./client > $D/c$i.txt 2>&1; echo $? > $D/c$i.rc ) &
done
wait

# Tableau de résultats
printf "%-8s %-8s %-8s %s\n" CLIENT DEMANDE RECU RESULTAT
ok=0
for i in $(seq 1 $N); do
  pid=$(awk '/demande/{print $2}' $D/c$i.txt)
  dem=$(awk '/demande/{print $6}' $D/c$i.txt)
  rec=$(awk '/a re/{print NF-5}' $D/c$i.txt)
  rc=$(cat $D/c$i.rc)
  if [ "$rc" = "124" ]; then res="BLOQUE"
  elif [ "$dem" = "$rec" ]; then res="OK"; ok=$((ok+1))
  else res="ERREUR"; fi
  printf "%-8s %-8s %-8s %s\n" "$pid" "$dem" "${rec:--}" "$res"
done
echo "----------------------------------"
echo "Bilan : $ok / $N clients corrects"
rm -rf 

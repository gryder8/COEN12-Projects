TIMEFORMAT='%3R';

if [ $# -eq 0 ]
  then
    echo "No arguments supplied. Supply an executable file name to test."
    exit -1
fi

echo "Testing $1"

declare -a books=("Bible" "Genesis" "GreenEggsAndHam" "HoundOfTheBaskervilles" "Macbeth" "TheCountOfMonteCristo" "TheSecretGarden" "TheWarOfTheWorlds" "TreasureIsland" "TwentyThousandLeagues")

for i in "${books[@]}"
do
    #/usr/bin/time -p -f "%e" ./$1 /scratch/coen12/$i.txt;
    echo "Testing $i.txt"
    time ./$1 /scratch/coen12/$i.txt > /dev/null 2>&1
    echo ""
done
exit 1





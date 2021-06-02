if [ $# -eq 0 ]
  then
    echo "No arguments supplied. Supply an executable file name to test."
    exit -1
fi

echo "Testing using $1"


echo "Testing Bible...";
./$1 /scratch/coen12/Bible.txt;
echo "";

echo "Testing Genesis...";
./$1 /scratch/coen12/Genesis.txt;
echo "";


echo "Testing Green Eggs and Ham...";
./$1 /scratch/coen12/GreenEggsAndHam.txt;
echo "";


echo "Testing Hound of the Baskervilles...";
./$1 /scratch/coen12/HoundOfTheBaskervilles.txt;
echo "";


echo "Testing Macbeth...";
./$1 /scratch/coen12/Macbeth.txt;
echo "";


echo "Testing The Count of Monte Cristo...";
./$1 /scratch/coen12/TheCountOfMonteCristo.txt;
echo "";


echo "Testing The Secret Garden...";
./$1 /scratch/coen12/TheSecretGarden.txt;
echo "";


echo "Testing The War of the Worlds...";
./$1 /scratch/coen12/TheWarOfTheWorlds.txt;
echo "";

echo "Testing Treasure Island...";
./$1 /scratch/coen12/TreasureIsland.txt;
echo "";

echo "Testing Twenty Thousand Leagues...";
./$1 /scratch/coen12/TwentyThousandLeagues.txt;
echo "";


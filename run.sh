
cd "$(dirname "$0")"/build

if [ ! -z "$1" ]
then
  echo "Generating MakeFile";
  cmake ..
fi
rm Fractals
make

cd ..
./build/Fractals
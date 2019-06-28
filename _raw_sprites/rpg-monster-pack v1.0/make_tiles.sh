files=$(ls *.png)
rm -rf tiles/ 2>&1
mkdir tiles/

size_multiple=6
multiple_in_pixels=`echo "48 * ${size_multiple}" | bc`
crop_size="${multiple_in_pixels}x${multiple_in_pixels}"

echo "multiple_in_pixels ${multiple_in_pixels}"
echo "crop_size ${crop_size}"

for i in $files; do 
  f=$(echo $i | awk -F. '{print $1}')
  echo Processing $f

  mkdir -p tiles/$f; 

  convert $i -crop 48x48 "tiles/$f/$f-tile.png"

  cd tiles/$f

  rm *3.png
  rm *7.png
  t=$(ls *.png | sort -V)


  for z in $t; do
    # mv $z in.png
    
    convert ../../background_checkerboard_48x48 -page +0+0 \( $z \) -flatten in.png

    convert in.png -interpolate Integer -filter point -resize ${size_multiple}00% $z 
    rm in.png
  done


  cd ../..

  t=$(ls tiles/$f/*.png | sort -V)

  convert -delay 15 -loop 0 -crop ${crop_size} -dispose Previous +repage $t tiles/$f-animated.gif
done
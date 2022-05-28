doxygen Doxyfile
cd latex
make
mv refman.pdf ../refman.pdf
cd ..
cp refman.pdf html/refman.pdf
git add .
git commit -m "automatic commit"
git push
git subtree push --prefix html origin gh-pages
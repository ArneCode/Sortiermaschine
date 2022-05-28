doxygen Doxyfile
cd latex
make
mv refman.pdf ../doc.pdf
cd ..
cp doc.pdf html/doc.pdf
git add .
git commit -m "automatic commit"
git push
git subtree push --prefix html origin gh-pages
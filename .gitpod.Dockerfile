FROM gitpod/workspace-full:2022-05-08-14-31-53
RUN sudo apt-get -y install flex
RUN git clone https://github.com/doxygen/doxygen.git
RUN cd doxygen &&\
    mkdir build &&\
    cd build &&\
    cmake -G "Unix Makefiles" .. &&\
    make &&\
    sudo make install
RUN sudo apt install -y graphviz && \
    sudo apt-get install -y texlive-fonts-recommended && \
    sudo apt-get install -y texlive-fonts-extra
RUN sudo apt -y install texlive-lang-german
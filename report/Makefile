all: build 

build: 
	latexmk report

clean:
	latexmk -c report

cleanall:
	latexmk -C report

view:
	latexmk -pvc report

rebuild: cleanall build

.PHONY: clean all view cleanall rebuild


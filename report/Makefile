MAIN_TEX = $(MAIN_TEX)
all: build 

build: 
	latexmk $(MAIN_TEX)

clean:
	latexmk -c $(MAIN_TEX)

cleanall:
	latexmk -C $(MAIN_TEX)

view:
	latexmk -pvc $(MAIN_TEX)

rebuild: cleanall build

.PHONY: clean all view cleanall rebuild


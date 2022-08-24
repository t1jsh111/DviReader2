# DVI Interprter

Dvi Interpreter is a C++ library to process DeVice Indepenent (DVI) files.
The DVI files are the native output for the TeX compiler, written by the famous computer Scientist Donald E. Knuth.

TeX is one of the oldest programs that is still in use as of today.
While, most users are familiar with the term LaTeX, this is in fact only a set of macros on top of TeX.
In the background, still the TeX engine is doing all the necessary computations, and generating the output file.
As of today, there are compilers such as PDFTex that can not only produce DVI files, but also PDF files directly from a TeX source.
These DVI files contain binary data that can be translated to graphical data. One could for example translate DVI files to PDF.

While PDFs have been standardized, the DVI files are still a format of interest.
The DVI files are more simplistic then PDFs are. For example, the DVI files do not embed the fonts that are used (but require them to be available on the machine).
This makes the production of DVI files more efficient, allowing for faster view of output file.
The latter could in particular be relevant for large document.
Using the \special command, it is possible to pass information in TeX directly onto the DVI files.
By exploiting this feature it is possible to build extensions on top of TeX.
We hope to research the possibilities of incremental live compilation (as done in BaKoMa Tex and TeXPad), storing of TeX Source code in DVI file, and preserving a one-to-one mapping of source code to DVI file after developing a DVI Reader.


It is clear that the DVI files, are still of interest.
In this (ongoing) project, an attempt is made to build an open-source DVI File viewer using the QT framework.




# Status


- DviInterpreter: Fully functional
- DviViewer: In progress.


# DviIntepreter

One construct a DviInterpreter as follows
```
int POINTSPERINCHX = 72;
int POINTSPERINCHY = 72;
DviInterpreter("~/documents/dvi_files/text.div", POINTSPERINCHX, POINTSPERINCHY, , PIXELSPERINCHX, PIXELSPERINCHY)
```

In typography, a point is the smallest unit of measure. The standard is defined as 1/72 of an inch.
In TeX a larger precision is needed, and the scaled point is introduced. This is defined as 1/65536 of a point.

In DVI files, we advance the write head with scaled points...

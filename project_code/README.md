devart
======

code for devart commission, tentatively titled "play the world".  

this should be at the root level of OF (0.8).  It's using OSX specific libraries at the moment, and will contain weird experiments with aubio, libsndfile, vamp, etc.  As we go, we'll start to label and mark things. 

(note this is a copy of this: https://github.com/ofzach/devart, the repo I'm doing development from.  That repo is LARGE as it has some binary files committed.  I will be cleaning that up and using this as submodule of that shortly)


- audio: raw audio files, analysis files, etc   (I've left this empty for now to keep the repo light)
- utils: command line tools such as vamp plugin runner
- noteDetection: experiments in segmentation of notes
- notePlayback: experiments in playing back note files

uses develop branch of ofxUI
(git checkout -b develop remotes/origin/develop)


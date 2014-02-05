##initial experiments

So my first question to answer is how to build this project?  Alot of times I find I get myself into trouble through the proposal and making the artwork is getting myself out of trouble.   

My initial apporach was very simple -- start finding things that "sound" like notes in audio and ripping them out, and build a tool that plays them back.  My challenge was how to classify the notes -- I might have hundreds or thousands, how do you go about figuring out what note they are? 

My first step was to use an library called aubio, which is an audio labeling library that includes several intersting pitch detection algorithms.  Pitch detection is a notoriously hard problem.  its intimately tied to our perception of pitch, and many algorithms get thrown off by polyphonic or noisy audio.  I wanted something fast, so this worked, but intuitively I felt like these pitch detectors would probably fail with very diverse tyes of audio.  For a single voice, they'd be fine, but for a huge band with alot of different instruments, they'd be schizophrenic.

the other question is how much context to put around a note?  For this first experiment, I start to play it just by ear.  It seems like half a second or so on either end of note is useful for understanding language.  I think it's going to be important to explore on context, specifically: 

- how do you include context without messing up the note too much? (especially if the context deviates from the note)
- how long of spoken language do you need to recognize where it's coming from?
- How do you "shape" these cut sounds, so they fade in and out smoothly?

here's the initial test.  It doesn't sound very good (!), but I sort of figured at the beginning it won't, and over the next weeks and months I will work hard to make it much more musical.  Also, note that there's some repetition here since I don't have a huge database of "note" samples yet.  That said, here is a short clip of me playing with that initial system:

https://soundcloud.com/thesystemis/jan-9-testing/

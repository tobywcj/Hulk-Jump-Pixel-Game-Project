#include "Tone.h"

Tone::Tone(PinName dacPin)
{
    dac = new AnalogOut(dacPin);
    ticker = new Ticker;
    timeout = new Timeout;
    note_timeout = new Timeout;
}

Tone::~Tone()
{
    delete dac;
    delete[] _sample_array;
    delete ticker;
    delete timeout;
    delete note_timeout;
}

void Tone::init()
{
    _n = 16;
    _sample_array = new float[_n];

    // create sample array for one period between 0.0 and 1.0
    for (int i = 0; i < _n ; i++) {
        _sample_array[i] = 0.5f + 0.5f*sin(i*2*PI/_n);
        //printf("y[%i] = %f\n",i,_sample_array[i]);
    }

}

void Tone::set_bpm(float bpm)
{
    _bpm = bpm;
}


void Tone::play(float frequency,float duration)
{
    // calculate time step between samples
    float dt = 1.0/(frequency*_n);
    // start from beginning of LUT
    _sample = 0;

    // setup ticker and timeout to stop ticker

    // the ticker repeats every dt to plat each sample in turn
    ticker->attach(callback(this, &Tone::ticker_isr), dt);
    // the timeout stops the ticker after the required duration
    timeout->attach(callback(this, &Tone::timeout_isr), duration );
}

void Tone::play_melody(int length,const int *notes,const int *durations,float bpm,bool repeat)
{
    // copy arguments to member variables
    _bpm = bpm;
    _notes = notes;  // pointer for array
    _durations = durations;  // pointer for array
    _melody_length = length;
    _repeat = repeat;

    _note = 0;  // start from first note

    play_next_note(); // play the next note in the melody
}


void Tone::play_next_note()
{
    // _note is the note index to play

    // calculate the duration and frequency of the note
    float duration = 60.0/(_bpm*_durations[_note]);
    float frequency = float(_notes[_note]);
    //printf("[%i] f = %f d = %f\n",_note,frequency,duration);

    // check if the note is not a space and if not then play the note
    if (frequency > 0) {
        play(frequency,duration);
    }

    // the timeout goes to the next note in the melody
    // double the duration to leave a bit of a gap in between notes to be better
    // able to distinguish them
    note_timeout->attach(callback(this, &Tone::note_timeout_isr), duration*2.0 );
}

// called when the next note needs playing
void Tone::note_timeout_isr()
{
    _note++; // go onto next note

    // if in repeat mode then reset the note counter when get to end of melody
    if (_repeat && _note == _melody_length) {
        _note=0;
    }

    // check if note is within the melody
    if (_note < _melody_length) {
        play_next_note();
    }
}

void Tone::ticker_isr()
{
    dac->write(_sample_array[_sample%_n]);  // use modulo to get index to play
    _sample++;  // increment the sample ready for next time
}

void Tone::timeout_isr()
{
    // stops the ticker to end the note
    ticker->detach();
}
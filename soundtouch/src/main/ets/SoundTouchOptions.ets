export interface SoundTouchOptions {
    /**
     * Change the sound tempo by n percents (n = -95.0 .. +5000.0 %)
     */
    tempo?: number;

    /**
     * Change the sound pitch by n semitones (n = -60.0 .. + 60.0 semitones)
     */
    pitch?: number;

    /**
     * Change the sound playback rate by n percents (n = -95.0 .. +5000.0 %)
     */
    rate?: number;

    /**
     * Use quicker tempo change algorithm. Gains speed but loses sound quality.
     */
    quick?: number;

    /**
     * Don't use anti-alias filtering in sample rate transposing. Gains speed but loses sound quality.
     */
    noAntiAlias?: number;

    /**
     * Detect the Beats-Per-Minute (BPM) rate of the sound and adjust the tempo to meet 'n' BPMs.
     * When this switch is applied, the "-tempo" switch is ignored. If "=n" is omitted, i.e.
     * switch "-bpm" is used alone, then the BPM rate is estimated and displayed,
     * but tempo not adjusted according to the BPM value.
     */
    detectBPM?: boolean;
    goalBPM?: number;

    /**
     * Tune algorithm for speech processing (default is for music)
     */
    speech?: number;
}
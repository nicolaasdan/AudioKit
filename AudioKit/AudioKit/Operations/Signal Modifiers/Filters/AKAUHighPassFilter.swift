//
//  AKAUHighPassFilter.swift
//  AudioKit
//
//  Autogenerated by scripts by Aurelius Prochazka. Do not edit directly.
//  Copyright (c) 2015 Aurelius Prochazka. All rights reserved.
//

import AVFoundation

/** AudioKit version of Apple's HighPassFilter Audio Unit */
public class AKAUHighPassFilter: AKOperation {
    
    private let cd = AudioComponentDescription(
        componentType: OSType(kAudioUnitType_Effect),
        componentSubType: OSType(kAudioUnitSubType_HighPassFilter),
        componentManufacturer: OSType(kAudioUnitManufacturer_Apple),
        componentFlags: 0,
        componentFlagsMask: 0)
    
    private var internalEffect = AVAudioUnitEffect()
    private var internalAU = AudioUnit()
    
    /** Cutoff Frequency (Hz) ranges from 10 to 22050 (Default: 6900) */
    public var cutoffFrequency: Float = 6900 {
        didSet {
            if cutoffFrequency < 10 {
                cutoffFrequency = 10
            }
            if cutoffFrequency > 22050 {
                cutoffFrequency = 22050
            }
            AudioUnitSetParameter(internalAU, kHipassParam_CutoffFrequency, kAudioUnitScope_Global, 0, cutoffFrequency, 0)
        }
    }
    
    /** Resonance (dB) ranges from -20 to 40 (Default: 0) */
    public var resonance: Float = 0 {
        didSet {
            if resonance < -20 {
                resonance = -20
            }
            if resonance > 40 {
                resonance = 40
            }
            AudioUnitSetParameter(internalAU, kHipassParam_Resonance, kAudioUnitScope_Global, 0, resonance, 0)
        }
    }
    
    /** Initialize the effect operation */
    public init(_ input: AKOperation) {
        super.init()
        internalEffect = AVAudioUnitEffect(audioComponentDescription: cd)
        output = internalEffect
        AKManager.sharedInstance.engine.attachNode(internalEffect)
        AKManager.sharedInstance.engine.connect(input.output!, to: internalEffect, format: nil)
        internalAU = internalEffect.audioUnit
    }
}
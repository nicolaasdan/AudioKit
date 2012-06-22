//
//  CSDPluck.h
//
//  Created by Adam Boulanger on 6/1/12.
//  Copyright (c) 2012 Hear For Yourself. All rights reserved.
//

#import "CSDOpcode.h"

//ARB - NOTE: started using weight in signatures for things that tend to be 0-1 and scaler/factor for >1
//ARB - NOTE: trying to deal with(hide) strange paramter that switches between different decay initializations which then 
//   recycles optional parameters for different init functions
//ARB - TODO: ival parameters don't need to be strings

@interface CSDPluck : CSDOpcode {
    CSDParamControl * amplitude;
    CSDParamControl * resamplingFrequency;
    CSDParamConstant * pitchDecayFrequency;
    CSDFunctionTable * cyclicDecayFunction;
    CSDParamConstant * decayMethod;
    CSDParamConstant * roughness;
    CSDParamConstant * stretchFactor;
    CSDParamConstant * currentSampleWeight;
    CSDParamConstant * previousSampleWeight;
    
    CSDParam * output;
}
@property (nonatomic, strong) CSDParam * output;

-(id)initWithAmplitude:(CSDParamControl *)amp
   ResamplingFrequency:(CSDParamControl *)freq
   PitchDecayFrequency:(CSDParamConstant *)pchDecayFreq
CyclicDecayFunctionTable:(CSDFunctionTable *)f
        RecursiveDecay:(BOOL)orSimpleDecay;

-(id)initWithAmplitude:(CSDParamControl *)amp
   ResamplingFrequency:(CSDParamControl *)freq
   PitchDecayFrequency:(CSDParamConstant *)pchDecayFreq
CyclicDecayFunctionTable:(CSDFunctionTable *)f  
StretchedAveragingDecay:(CSDParamConstant *)stretchScaler;

-(id)initWithAmplitude:(CSDParamControl *)amp
   ResamplingFrequency:(CSDParamControl *)freq
   PitchDecayFrequency:(CSDParamConstant *)pchDecayFreq
CyclicDecayFunctionTable:(CSDFunctionTable *)f
       SimpleDrumDecay:(CSDParamConstant *)roughWeight;

-(id)initWithAmplitude:(CSDParamControl *)amp
   ResamplingFrequency:(CSDParamControl *)freq
   PitchDecayFrequency:(CSDParamConstant *)pchDecayFreq
CyclicDecayFunctionTable:(CSDFunctionTable *)f
    StretchedDrumDecay:(CSDParamConstant *)roughWeight
         StretchFactor:(CSDParamConstant *)stretchScaler;

-(id)initWithAmplitude:(CSDParamControl *)amp
   ResamplingFrequency:(CSDParamControl *)freq
   PitchDecayFrequency:(CSDParamConstant *)pchDecayFreq
CyclicDecayFunctionTable:(CSDFunctionTable *)f
  WeightedAverageDecay:(CSDParamConstant *)currSampleWeight
         StretchFactor:(CSDParamConstant *)prevSampleWeight;

@end

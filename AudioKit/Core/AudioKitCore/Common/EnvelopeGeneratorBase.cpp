//
//  EnvelopeGeneratorBase.cpp
//  AudioKit Core
//
//  Created by Shane Dunne, revision history on Github.
//  Copyright © 2018 AudioKit. All rights reserved.
//
#include "EnvelopeGeneratorBase.hpp"
#include <cmath>

namespace AudioKitCore
{

    void ExponentialSegmentGenerator::reset(double initialValue, double targetValue, double tco, int segmentLengthSamples)
    {
        output = segmentLengthSamples > 0 ? initialValue : targetValue;
        target = targetValue;
        isHorizontal = targetValue == initialValue;
        isLinear = tco <= 0.0;
        isRising = targetValue > initialValue;

        printf("initVal %f targetVal %f output %f target %f  len %i\n\n", initialValue, targetValue, output, target, segmentLengthSamples);
        if (isHorizontal)
        {
            tcount = 0;
            segLength = segmentLengthSamples;
        }
        else if (isLinear)
        {
            if (segmentLengthSamples <= 0)
                coefficient = target - output;
            else
                coefficient = (targetValue - initialValue) / segmentLengthSamples;
        }
        else
        {
            if (segmentLengthSamples == 0)
            {
                coefficient = 0.0;
                offset = target;
            }
            else
            {
                // Correction to Pirkle (who uses delta = 1.0 always)
                // According to Redmon (who only discusses the delta = 1.0 case), delta should be defined thus
                double delta = abs(targetValue - initialValue);
                coefficient = exp(-log((delta + tco) / tco) / segmentLengthSamples);
                if (isRising)
                    offset = (target + tco) * (1.0 - coefficient);
                else
                    offset = (target - tco) * (1.0 - coefficient);
            }
        }
    }

    void MultiSegmentEnvelopeGenerator::setupCurSeg()
    {
        SegmentDescriptor seg = (*segments)[curSegIndex];
        double initValue = seg.initialValue;
        double targetValue = seg.finalValue;
        printf("autoAdvance %i: init %f final %f len %i - init %f target %f \n", curSegIndex, seg.initialValue, seg.finalValue, seg.lengthSamples, initValue, targetValue);
        ExponentialSegmentGenerator::reset(initValue, targetValue, seg.tco, seg.lengthSamples);
    }

    void MultiSegmentEnvelopeGenerator::setupCurSeg(double initValue)
    {
        SegmentDescriptor seg = (*segments)[curSegIndex];
        double targetValue = seg.finalValue;
        bool isHorizontal = seg.initialValue == seg.finalValue;
        if (isHorizontal) { // if flat (hold) then use same value
            targetValue = initValue;
        }
        printf("manualAdvance %i: init %f final %f len %i - init %f target %f \n", curSegIndex, seg.initialValue, seg.finalValue, seg.lengthSamples, initValue, targetValue);
        ExponentialSegmentGenerator::reset(initValue, targetValue, seg.tco, seg.lengthSamples);
    }

    void MultiSegmentEnvelopeGenerator::reset(Descriptor* pDesc, int initialSegmentIndex)
    {
        segments = pDesc;
        curSegIndex = initialSegmentIndex;
        setupCurSeg();
    }

    void MultiSegmentEnvelopeGenerator::startAtSegment(int segIndex) //puts the envelope in a 'fresh' state, allows sudden jumps to first segment
    {
        curSegIndex = segIndex;
        skipEmptySegments();
        SegmentDescriptor& seg = (*segments)[curSegIndex];
        setupCurSeg(seg.initialValue); // we are restarting, not advancing, so  always start from the first vlaue we get to
    }

    void MultiSegmentEnvelopeGenerator::advanceToSegment(int segIndex) //advances w/ awareness of state, so as to not make sudden jumps
    {
        curSegIndex = segIndex;
        skipEmptySegments();
        setupCurSeg(output); //we are advancing, not restarting, so always start from the value we are currently at
    }

    void MultiSegmentEnvelopeGenerator::skipEmptySegments() //skips over any segment w/ length 0, so as to not influence the state of the envelope
    {
        SegmentDescriptor seg = (*segments)[curSegIndex];
        int length = seg.lengthSamples;
        while (length == 0) { //skip any segments that are 0-length
            printf("skipping seg %i\n",  curSegIndex);
            curSegIndex++;
            seg = (*segments)[curSegIndex];
            length = seg.lengthSamples;
        }
    }

}

/*

 Copyright (C) 2016  Queen Mary University of London 
 Author: Fiore Martin

 This file is part of Collidoscope.
 
 Collidoscope is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once 

namespace collidoscope {


/* 
 * An ASR envelope with linear shape. It is modeled after the STK envelope classes.
 * The tick() method advances the computation of the envelope one sample and returns the computed sample
 * The class is templated for the type of the samples that each tick of the envelope produces. 
 *
 * Client classes can set/get the current state of the envelope with the
 * respective getter/setter methods
 *
 */
template <typename T>
class EnvASR
{
public:

    /** Possible states of the envelope. Idle means the envelope ouputs 0 */
    enum class State {
        eAttack,
        eSustain,
        eRelease,
        eIdle // before attack after release  
    };

    EnvASR( T sustainLevel, T attackTime, T releaseTime, std::size_t sampleRate ) :
        mSustainLevel( sustainLevel ),
        mState( State::eIdle ),
        mValue( 0 )
        
    {
        if ( attackTime <= 0 )
            attackTime = T( 0.001 );

        if ( releaseTime <= 0 )
            releaseTime = T( 0.001 );
        
        mAttackRate =  T( 1.0 ) / (attackTime * sampleRate);
        mReleaseRate = T( 1.0 ) / (releaseTime * sampleRate);
    }

    /** Produces one sample worth of envelope */
    T tick()
    {

        switch ( mState )
        {

        case State::eIdle: {
            mValue = 0;
        };
            break;

        case State::eAttack: {
            mValue += mAttackRate;
            if ( mValue >= mSustainLevel ){
                mValue = mSustainLevel;
                mState = State::eSustain;
            }
        };
            break;

        case State::eRelease:
            mValue -= mReleaseRate;
            if ( mValue <= 0 ){
                mValue = 0;
                mState = State::eIdle;
            }
            break;
        default:
            break;
        }

        return mValue;

    }

    State getState() const
    {
        return mState;
    }

    void setState( State state )
    {
        mState = state;
    }

private:
    T mSustainLevel;
    T mAttackRate;
    T mReleaseRate;

    // output
    T mValue;

    State mState;

};


}

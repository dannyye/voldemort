/* -*- C++ -*-; c-basic-offset: 4; indent-tabs-mode: nil */
/*!
 * @file VectorClock.h
 * @brief Interface definition file for VectorClock
 */
/* Copyright (c) 2009 Webroot Software, Inc.
 *
 * Licensed under the Apache License, VectorClock 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

#ifndef VECTORCLOCK_H
#define VECTORCLOCK_H

#include <stdint.h>
#include <voldemort/Version.h>
#include <list>
#include <utility>
#include <ostream>

namespace Voldemort {

/** 
 * A vector of the number of writes mastered by each node. The vector is stored
 * sparsly, since, in general, writes will be mastered by only one node. This
 * means implicitly all the versions are at zero, but we only actually store
 * those greater than zero.
 * 
 */
class VectorClock: public Version
{
 public:
    /**
     * Construct an empty VectorClock
     */
    VectorClock();
    
    /**
     * Construct an empty VectorClock using the provided timestamp
     *
     * @param timestamp the timestamp to initialize to
     */
    VectorClock(uint64_t timestamp);

    /**
     * Construct a VectorClock using the provided versions and timestamp
     *
     * @param versions the versions to include in the clock
     * @param timestamp the timestamp to initialize to
     */
    VectorClock(std::list<std::pair<short, uint64_t> >* versions, uint64_t timestamp);

    virtual ~VectorClock();

    /**
     * Virtual copy constructor allocates a new VectorClock object
     * containing the same information as this one.
     */
    virtual VectorClock* copy();

    /**
     * Return whether or not the given vectorClock preceeded this one,
     * succeeded it, or is concurrant with it
     *
     * @param v The other vectorClock
     * @return one of the Occurred values
     */
    virtual Occurred compare(Version* v);

    /**
     * Compare two VectorClocks, the outcomes will be one of the
     * following: -- Clock 1 is BEFORE clock 2 if there exists an i
     * such that c1(i) <= c(2) and there does not exist a j such that
     * c1(j) > c2(j). -- Clock 1 is CONCURRENT to clock 2 if there
     * exists an i, j such that c1(i) < c2(i) and c1(j) > c2(j) --
     * Clock 1 is AFTER clock 2 otherwise
     * 
     * @param v1 The first VectorClock
     * @param v2 The second VectorClock
     * @return one of the Occurred values
     */
    static Occurred compare(VectorClock* v1, VectorClock* v2);

    /**
     * Get the list of version entries
     */
    const std::list<std::pair<short, uint64_t> >* getEntries() const;

    /**
     * Get the timestamp
     */
    uint64_t getTimestamp();

    /** 
     * Stream insertion operator for cluster 
     *
     * @param output the stream
     * @param vc the @ref VectorClock object
     * @return the stream
     */
    friend std::ostream& operator<<(std::ostream& output, 
                                    const VectorClock& vc);

private:
    // Disable copy constructor
    VectorClock(const VectorClock& v) { }

    /** 
     * The time of the last update on the server on which the update
     * was performed
     */
    uint64_t timestamp;

    /**
     * The versions contained in this clock
     */
    std::list<std::pair<short, uint64_t> >* versions;
};

} /* namespace Voldemort */

#endif // VECTORCLOCK_H

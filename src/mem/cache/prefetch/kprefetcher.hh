/*
 * Copyright (c) 2012-2013, 2015 ARM Limited
 * All rights reserved
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Copyright (c) 2005 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Ron Dreslinski
 */

/**
 * @file
 * Describes a strided prefetcher.
 */

#ifndef __MEM_CACHE_PREFETCH_KPREFETCHER_HH__
#define __MEM_CACHE_PREFETCH_KPREFETCHER_HH__

#include <deque>

#include "mem/cache/prefetch/queued.hh"
#include "params/KPrefetcher.hh"

class KPrefetcher : public QueuedPrefetcher
{
  protected:
    struct TableParameters{
        Request::Flags flags;
        int prftablesize;
        int maxprfofs;
        int degree;
        int slowstart;
        int prio;
    };
    struct KEntry
    {
        KEntry() : pktAddr(0), prfAddr(0), incr(true), isStore(false)
        { }

        Addr pktAddr;
        Addr prfAddr;
        bool incr;
        bool isStore;
    };
    using Kpftable = std::deque<KEntry>;
    Kpftable entriesl1;
    Kpftable entriesl2;
    TableParameters l1param, l2param;
    bool writeprefetch;
    bool hpctag;
    void
    calculateTable(Kpftable &entries, const PacketPtr &pkt,
                                std::vector<AddrPriority> &addresses,
                                const TableParameters &prm);
  public:

    KPrefetcher(const KPrefetcherParams *p);
    void calculatePrefetch(const PacketPtr &pkt,
                           std::vector<AddrPriority> &addresses);
};

#endif // __MEM_CACHE_PREFETCH_KPREFETCHER_HH__

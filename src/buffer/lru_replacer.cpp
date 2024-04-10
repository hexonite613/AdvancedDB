//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"
#include <cassert>

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages){}

LRUReplacer::~LRUReplacer() = default;

bool LRUReplacer::Victim(frame_id_t *frame_id) {
  lru_mutex.lock();
  
  if(unpinned_frames.empty()==0){
    *frame_id = unpinned_frames.front();
    unpinned_frames.pop_front();
    lru_mutex.unlock();
    return true;
  }

  lru_mutex.unlock();
  return false;
}


void LRUReplacer::Pin(frame_id_t frame_id) {
  lru_mutex.lock();

  std::list<frame_id_t>::iterator i;
  for (i = unpinned_frames.begin(); i!=unpinned_frames.end(); i++) {
      if (*i==frame_id) {
          unpinned_frames.erase(i);
          break;    
      }
  }

  lru_mutex.unlock();
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
    lru_mutex.lock();

    bool check = false;

    std::list<frame_id_t>::iterator i;
    for (i = unpinned_frames.begin(); i!=unpinned_frames.end(); i++) {
        if (*i==frame_id) {
            check=true;
            break;
        }
    }

    if (check==0) {
        unpinned_frames.push_back(frame_id);
    }

    lru_mutex.unlock();
}

size_t LRUReplacer::Size() { return unpinned_frames.size(); }

}  // namespace bustub

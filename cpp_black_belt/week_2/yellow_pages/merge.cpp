#include "yellow_pages.h"

#include <algorithm>
#include <tuple>

#if 1
#define MERGE_NOT_REPEATED(class__,field__)                                   \
{                                                                             \
  class__ addr;                                                               \
  uint32_t cur_prio = 0;                                                      \
  for(const auto& signal : signals) {                                         \
    if(signal.has_company() && signal.company().has_##field__()) {            \
      auto provider_prio = providers.at(signal.provider_id()).priority();     \
      if(provider_prio > cur_prio) {                                          \
        cur_prio = provider_prio;                                             \
        addr = signal.company().field__();                                    \
      }                                                                       \
    }                                                                         \
  }                                                                           \
  *res.mutable_##field__() = addr;                                            \
}
#endif

#define MERGE_REPEATED(class__,field__)                                       \
{                                                                             \
  std::vector<class__> temp__;                                                \
  std::set<std::string> uniques;                                              \
  uint32_t cur_prio = 0;                                                      \
  for(const auto& signal : signals) {                                         \
    if(signal.has_company() && signal.company().field__##_size()) {           \
      auto provider_prio = providers.at(signal.provider_id()).priority();     \
      if(provider_prio > cur_prio) {                                          \
        cur_prio = provider_prio;                                             \
        temp__.clear();                                                       \
      }                                                                       \
      if(provider_prio == cur_prio) {                                         \
        for(const auto& name : signal.company().field__()) {                  \
          std::string check_unique;                                           \
          name.AppendToString(&check_unique);                                 \
          if(uniques.insert(check_unique).second)                             \
            temp__.push_back(name);                                           \
        }                                                                     \
      }                                                                       \
    }                                                                         \
  }                                                                           \
  for(auto& temp_ : temp__) {                                                 \
    *res.add_##field__() = std::move(temp_);                                  \
  }                                                                           \
}

namespace YellowPages {

  Company Merge(const Signals& signals, const Providers& providers) {
    if(signals.empty()) return Company();
    Company res;
    MERGE_NOT_REPEATED(Address,address);
    MERGE_NOT_REPEATED(WorkingTime,working_time);
    MERGE_REPEATED(Name,names);
    MERGE_REPEATED(Phone,phones);
    MERGE_REPEATED(Url,urls);    
    return std::move(res);
  }

}

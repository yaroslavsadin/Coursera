#include "yellow_pages.h"

#include <algorithm>
#include <tuple>
#include "message_differencer.h"

namespace YellowPages {

  Company Merge(const Signals& signals, const Providers& providers) {
    if(signals.empty()) return Company();

    Company res;
    // Dealing with Address
    {
      Address addr;
      uint32_t cur_prio = 0;
      for(const auto& signal : signals) {
        if(signal.has_company() && signal.company().has_address()) {
          auto provider_prio = providers.at(signal.provider_id()).priority();
          if(provider_prio > cur_prio) {
            cur_prio = provider_prio;
            addr = signal.company().address();
          }
        }
      }
      *res.mutable_address() = addr;
    }
    // Dealing with names
    {
      std::vector<Name> names;
      std::set<std::string> uniques;
      uint32_t cur_prio = 0;
      for(const auto& signal : signals) {
        if(signal.has_company() && signal.company().names_size()) {
          auto provider_prio = providers.at(signal.provider_id()).priority();
          if(provider_prio > cur_prio) {
            cur_prio = provider_prio;
            names.clear();
          }
          if(provider_prio == cur_prio) {
            for(const auto& name : signal.company().names()) {
              std::string check_unique;
              name.AppendToString(&check_unique);
              if(uniques.insert(check_unique).second)
                names.push_back(name);
            }
          }
        }
      }
      for(auto& name : names) {
        *res.add_names() = std::move(name);
      }
    }
    // Dealing with phones
    {
      std::vector<Phone> phones;
      std::set<std::string> uniques;
      uint32_t cur_prio = 0;
      for(const auto& signal : signals) {
        if(signal.has_company() && signal.company().phones_size()) {
          auto provider_prio = providers.at(signal.provider_id()).priority();
          if(provider_prio > cur_prio) {
            cur_prio = provider_prio;
            phones.clear();
          }
          if(provider_prio == cur_prio) {
            for(const auto& name : signal.company().phones()) {
              std::string check_unique;
              name.AppendToString(&check_unique);
              if(uniques.insert(check_unique).second)
                phones.push_back(name);
            }
          }
        }
      }
      for(auto& name : phones) {
        *res.add_phones() = std::move(name);
      }
    }
    // Dealing with URLs
    {
      std::vector<Url> urls;
      std::set<std::string> uniques;
      uint32_t cur_prio = 0;
      for(const auto& signal : signals) {
        if(signal.has_company() && signal.company().urls_size()) {
          auto provider_prio = providers.at(signal.provider_id()).priority();
          if(provider_prio > cur_prio) {
            cur_prio = provider_prio;
            urls.clear();
          }
          if(provider_prio == cur_prio) {
            for(const auto& name : signal.company().urls()) {
              std::string check_unique;
              name.AppendToString(&check_unique);
              if(uniques.insert(check_unique).second)
                urls.push_back(name);
            }
          }
        }
      }
      for(auto& name : urls) {
        *res.add_urls() = std::move(name);
      }
    }
    // Dealing with WorkingTime
    {
      WorkingTime working_time;
      uint32_t cur_prio = 0;
      for(const auto& signal : signals) {
        if(signal.has_company() && signal.company().has_working_time()) {
          auto provider_prio = providers.at(signal.provider_id()).priority();
          if(provider_prio > cur_prio) {
            cur_prio = provider_prio;
            working_time = signal.company().working_time();
          }
        }
      }
      *res.mutable_working_time() = working_time;
    }
    
    return std::move(res);
  }

}

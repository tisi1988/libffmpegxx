#include "public/utils/AVOptions.h"

#include "public/utils/Logger.h"
#include "utils/exception.h"

extern "C" {
#include <libavutil/dict.h>
}

namespace libffmpegxx {
namespace utils {
AVDictionary *toAVDictionary(AVOptions const &options) {
  AVDictionary *opts = nullptr;

  for (auto &&[key, value] : options) {
    std::string valueStr{""};

    if (std::holds_alternative<int>(value)) {
      valueStr = std::to_string(std::get<int>(value));
    } else {
      valueStr = std::get<std::string>(value);
    }

    int const err = av_dict_set(&opts, key.c_str(), valueStr.c_str(),
                                AV_DICT_DONT_OVERWRITE);
    if (err < 0) {
      LOG_FATAL_FFMPEG_ERR("Error while parsing option " + key, err)
    }
  }

  return opts;
}

AVOptions fromAVDictionary(AVDictionary *dict) {
  AVOptions options;

  auto const entryCount = av_dict_count(dict);
  if (entryCount == 0) {
    return options;
  }

  AVDictionaryEntry *prev{nullptr};
  AVDictionaryEntry *currEntry{nullptr};
  int count{0};

  do {
    currEntry = av_dict_get(dict, "", prev, AV_DICT_IGNORE_SUFFIX);

    try {
      int const value = std::stoi(currEntry->value);
      options.insert({currEntry->key, value});
    } catch (...) {
      options.insert({currEntry->key, currEntry->value});
    }

    prev = currEntry;
    ++count;
  } while (prev != nullptr && count < entryCount);

  return options;
}
}; // namespace utils
}; // namespace libffmpegxx

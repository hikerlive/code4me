#ifndef __AUDIO_PLAYER_MANAGER_H
#define __AUDIO_PLAYER_MANAGER_H

class AudioPlayerManager {
public:
  AudioPlayerManager();
  ~AudioPlayerManager();

public:
  void Start();
  void Stop();
  void Prepare();
  void SeekTo();
  void GetDuration();
};

#endif /* __AUDIO_PLAYER_MANAGER_H */

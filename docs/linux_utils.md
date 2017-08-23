# Linux utils

## Convert MP3 to ogg
```bash
ffmpeg -i input.mp3 -c:a libvorbis -q:a 4 output.ogg
```

## Rename files in bulk
```bash
rename 's/^Cowboy_2Gusn_Walk_.*_(.*)\.png/$1.png/' *
```

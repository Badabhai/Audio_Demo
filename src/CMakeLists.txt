#app sources
file(GLOB_RECURSE  audio_demo_app_srcs  ./*.c)
target_sources(audioDemo PRIVATE ${audio_demo_app_srcs})
HEADER_DIRECTORIES(audio_demo_private_include_dir ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(audioDemo PRIVATE ${audio_demo_private_include_dir})

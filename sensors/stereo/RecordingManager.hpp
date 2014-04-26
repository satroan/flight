#ifndef RECORDING_MANAGER_H_
#define RECORDING_MANAGER_H_

#include "opencv-stereo-util.hpp"
#include "hud.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#define RINGBUFFER_SIZE (120*50) // number of seconds to allocate for recording * framerate

using namespace std;
using namespace cv;

class RecordingManager {

    public:
        
        RecordingManager();
        ~RecordingManager();
        
        void Init(OpenCvStereoConfig stereo_config, Mat image_left, Mat image_right);
        
        void AddFrames(Mat image_left, Mat image_right);
        
        void FlushBufferToDisk();
        
        void BeginNewRecording();
        
        bool LoadVideoFiles(string video_file_left, string video_file_right);
        
        void GetPlaybackFrame(Mat &left_image, Mat &right_image);
        
        void SetRecordingOn(bool x) { recording_on_ = x; }
        
        bool SetPlaybackVideoDirectory(string video_directory);
        void SetPlaybackVideoNumber(int video_number, long long timestamp);
        void SetPlaybackFrameNumber(int frame_number);
        int GetPlaybackFrameNumber() { return file_frame_number_; }
        
        bool UsingLiveCameras() { return !using_video_from_disk_; }
        
        int GetRecFrameNumber() { return recNumFrames; }
        int GetRecVideoNumber() { return video_number; }
        
        void SetHudNumbers(Hud hud);
        
        void RecFrameHud(Mat hud_frame);
        
        void SetQuietMode(bool x) { quiet_mode_ = x; }
        
    private:
        
        string SetupVideoWriterPGM(string dirnamePrefix, bool increment_number);
        VideoWriter SetupVideoWriterAVI(string filenamePrefix, Size frameSize, bool increment_number, bool is_color = false);
        
        int LoadVideoFileFromDir(long long timestamp, int video_number);
    
        void GetFramePGM(Mat &left_image, Mat &right_image);
        void GetFrameAVI(Mat &left_image, Mat &right_image);
        
        string GetNextVideoFilename(string filename_prefix, bool use_pgm, bool increment_number);
        int GetNextVideoNumber(bool use_pgm, bool increment_number);
        
        int MatchVideoFile(string directory, string datestr, bool using_avi = false, int match_number = -1);
        int GetSkipNumber(string filename);
        string GetDateSring();
        string CheckOrCreateDirectory(string dir);
        
        
    
        OpenCvStereoConfig stereo_config_;
        
        // allocate a huge array for a ringbuffer
        Mat ringbufferL[RINGBUFFER_SIZE];
        Mat ringbufferR[RINGBUFFER_SIZE];
        
        VideoCapture *left_video_capture_;
        VideoCapture *right_video_capture_;
        
        string pgm_left_dir_; // directory for the "loaded" pgm files
        string pgm_right_dir_;
        
        VideoWriter record_hud_writer_;
        
        int recNumFrames;
        int video_number;
        bool recording_on_;
        bool reading_pgm_;
        
        bool using_video_directory_;
        string video_directory_;
        
        bool using_video_from_disk_;
        bool record_hud_setup_;
        
        int current_video_number_;
        
        int file_frame_number_;
        int file_frame_skip_;
        
        bool quiet_mode_;
};


#endif

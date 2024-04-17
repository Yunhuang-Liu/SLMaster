#include "monoInterzoneSinusFourGrayscalePattern.h"

#include "../../algorithm/algorithm.h"

using namespace cv;
using namespace cv::cuda;
using namespace std;

namespace slmaster {

using namespace algorithm;

namespace cameras {

static MonoInterzoneSinusFourGrayscalePattern::Params params__;

MonoInterzoneSinusFourGrayscalePattern::MonoInterzoneSinusFourGrayscalePattern() {}

std::shared_ptr<Pattern>
MonoInterzoneSinusFourGrayscalePattern::create(const Params &params) {
    params__ = params;

    return std::make_shared<MonoInterzoneSinusFourGrayscalePattern>();
}

bool MonoInterzoneSinusFourGrayscalePattern::generate(vector<Mat> &imgs) const {

    InterzoneSinusFourGrayscalePattern::Params params;
    params.width = params__.width_;
    params.height = params__.height_;
    params.nbrOfPeriods = params__.cycles_;
    params.horizontal = params__.horizontal_;
    params.confidenceThreshold = params__.confidenceThreshold_;
    params.shiftTime = params__.shiftTime_;

    return InterzoneSinusFourGrayscalePattern::create(params)->generate(imgs);
}

bool MonoInterzoneSinusFourGrayscalePattern::decode(
    const vector<vector<Mat>> &patternImages, Mat &depthMap,
    const bool isGpu) const {
    CV_Assert(patternImages.size() >= 1);

#ifdef OPENCV_WITH_CUDA_MODULE
    if (isGpu) {
        /*
        SinusCompleGrayCodePatternGPU::Params params;
        params.shiftTime = params__.shiftTime_;
        params.confidenceThreshold = params__.confidenceThreshold_;
        params.height = params__.height_;
        params.width = params__.width_;
        params.nbrOfPeriods = params__.cycles_;
        params.horizontal = params__.horizontal_;

        auto pattern = SinusCompleGrayCodePatternGPU::create(params);

        vector<vector<Mat>> imgsDivided(2); // leftPhaseImgs, leftGrayImgs
        for (int i = 0; i < patternImages[0].size(); ++i) {
            if (i < params.shiftTime) {
                imgsDivided[0].push_back(patternImages[0][i]);
            } else {
                imgsDivided[1].push_back(patternImages[0][i]);
            }
        }

        vector<Mat> imgsDividedMerged(2);
        parallel_for_(Range(0, 2), [&](const Range &range) {
            for (int i = range.start; i < range.end; ++i) {
                merge(imgsDivided[i], imgsDividedMerged[i]);
            }
        });

        vector<GpuMat> imgsDividedMergedDev(2);
        GpuMat wrappedMapDev, confidenceMapDev, unwrappedMapDev;
        imgsDividedMergedDev[0].upload(imgsDividedMerged[0]);
        imgsDividedMergedDev[1].upload(imgsDividedMerged[1]);
        pattern->computeWrappedAndConfidenceMap(
            imgsDividedMergedDev[0], wrappedMapDev, confidenceMapDev);
        pattern->unwrapPhaseMap(imgsDividedMergedDev[1], wrappedMapDev,
                                confidenceMapDev, unwrappedMapDev,
                                params.confidenceThreshold);

        GpuMat depthMapDev;
        cuda::reverseCamera(unwrappedMapDev, params__.PL1_, params__.PR4_,
                            params__.minDepth_, params__.maxDepth_,
                            static_cast<float>(params__.horizontal_
                                                   ? params__.height_
                                                   : params__.width_) /
                                params__.cycles_,
                            depthMapDev, params__.horizontal_);

        depthMapDev.download(depthMap);
        return true;
        */
        return false;
    }
#endif

    InterzoneSinusFourGrayscalePattern::Params params;
    params.shiftTime = params__.shiftTime_;
    params.confidenceThreshold = params__.confidenceThreshold_;
    params.height = params__.height_;
    params.width = params__.width_;
    params.nbrOfPeriods = params__.cycles_;
    params.horizontal = params__.horizontal_;

    auto pattern = InterzoneSinusFourGrayscalePattern::create(params);
    Mat wrappedMap, confidenceMap, floorMap, unwrappedMap;

    pattern->computePhaseMap(patternImages[0], wrappedMap);
    pattern->computeConfidenceMap(patternImages[0], confidenceMap);
    pattern->computeFloorMap(patternImages[0], confidenceMap, floorMap);
    pattern->unwrapPhaseMap(wrappedMap, floorMap, confidenceMap, unwrappedMap);

    reverseCamera(unwrappedMap, params__.PL1_, params__.PR4_,
                  params__.minDepth_, params__.maxDepth_,
                  static_cast<float>(params__.horizontal_ ? params__.height_
                                                          : params__.width_) /
                      params__.cycles_,
                  depthMap, params__.horizontal_);

    return true;
}
} // namespace cameras
} // namespace slmaster
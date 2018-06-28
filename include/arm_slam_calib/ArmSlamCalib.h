/*
 * ArmSlamCalib.h
 *
 *  Created on: Jan 26, 2016
 *      Author: mklingen
 */

#ifndef ARMSLAMCALIB_H_
#define ARMSLAMCALIB_H_

#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>

#include <gtsam/nonlinear/NonlinearFactorGraph.h>
#include <arm_slam_calib/EncoderFactor.h>
#include <arm_slam_calib/RobotProjectionFactor.h>
#include <gtsam/nonlinear/LevenbergMarquardtOptimizer.h>
#include <gtsam/nonlinear/Marginals.h>
#include <arm_slam_calib/DriftFactor.h>

#include <dart/dart.hpp> // Changed from dart.h
#include <dart/utils/urdf/DartLoader.hpp>

#include <stdio.h>
#include <ros/ros.h>
#include <fstream>
#include <aikido/rviz/InteractiveMarkerViewer.hpp>
#include <aikido/io/CatkinResourceRetriever.hpp> // Changed from aikido/util/CatkinResourceRetriever.hpp

// Camera observations of landmarks (i.e. pixel coordinates) will be stored as Point2 (x, y).
#include <gtsam/geometry/Point2.h>

// Inference and optimization
#include <gtsam/inference/Symbol.h>
#include <gtsam/nonlinear/ISAM2.h>
#include <gtsam/nonlinear/Values.h>
#include <gtsam/nonlinear/NonlinearFactorGraph.h>

// SFM-specific factors
#include <gtsam/slam/PriorFactor.h>
#include <gtsam/slam/GeneralSFMFactor.h> // does calibration !
#include <gtsam/slam/ProjectionFactor.h>

#include <arm_slam_calib/FrontEnd.h>
#include <arm_slam_calib/Landmark.h>
#include <arm_slam_calib/RobotProjectionFactor.h>
#include <joint_state_recorder/JointStateRecorder.h>
#include <tf/transform_broadcaster.h>

#include <arm_slam_calib/PerlinNoise.h>

#include <memory>

namespace gtsam
{

    class ArmSlamCalib
    {
        public:

            enum OptimizationMode
            {
                BatchLM,
                BatchDogleg,
                ISAM
            };

            class Params
            {
                public:
                    Params() :
                        trajectorySize(1000),
                        numLandmarksX(25),
                        numLandmarksY(25),
                        landmarkSizeX(10),
                        landmarkSizeY(10),
                        fx(640),
                        fy(640),
                        cx(640 / 2),
                        cy(480 /2),
                        encoderNoiseLevel(0.1),
                        velocityNoiseLevel(0.01),
                        extrinsicNoiseLevel(0.1),
                        extrinsicRotNoiseLevel(0.1),
                        landmarkNoiseLevel(50.0),
                        driftNoise(0.05),
                        initialPosePriorNoise(0.00001),
                        projectionNoiseLevel(1.0),
                        extrinsicInitialGuess(gtsam::Pose3::identity()),
                        runRansac(true),
                        saveImages(false),
                        /* DISABLE_POINT_CLOUDS
                        generateStitchedPointClouds(true),
                        generateCurrentPointCloud(true),
                        */
                        computeExtrinsicMarginals(false),
                        drawEstimateRobot(false),
                        useDeadBand(false),
                        addDriftNoise(false),
                        deadBandSize(0.05),
                        optimizationMode(BatchDogleg),
                        simulated(false),
                        doOptimize(true),
                        useVelocityNoise(false),
                        useEncoderPositions(true),
                        addSimPerlinNoise(true),
                        simPerlinMagnitude(0.2),
                        simPerlinFrequency(0.5)
                    {

                    }

                    virtual ~Params()
                    {

                    }

                    void InitializeNodehandleParams(const ros::NodeHandle& nh);

                    size_t trajectorySize;
                    size_t numLandmarksX;
                    size_t numLandmarksY;
                    double landmarkSizeX;
                    double landmarkSizeY;
                    double fx;
                    double fy;
                    double cx;
                    double cy;
                    double encoderNoiseLevel;
                    double velocityNoiseLevel;
                    double extrinsicNoiseLevel;
                    double extrinsicRotNoiseLevel;
                    double landmarkNoiseLevel;
                    double projectionNoiseLevel;
                    double driftNoise;
                    double initialPosePriorNoise;
                    gtsam::Pose3 extrinsicInitialGuess;
                    bool runRansac;
                    bool saveImages;
                    bool generateStitchedPointClouds;
                    bool generateCurrentPointCloud;
                    bool drawEstimateRobot;
                    bool computeExtrinsicMarginals;
                    bool useDeadBand;
                    bool addDriftNoise;
                    bool simulated;
                    bool doOptimize;
                    bool useVelocityNoise;
                    bool useEncoderPositions;
                    double deadBandSize;
                    bool addSimPerlinNoise;
                    double simPerlinMagnitude;
                    double simPerlinFrequency;
                    OptimizationMode optimizationMode;
            };


            struct CalibrationError
            {
                    double landmarkError;
                    double jointAngleError;
                    double extrinsicError;
                    double cameraPoseError;
            };


            ArmSlamCalib(const ros::NodeHandle& nh, const std::shared_ptr<std::mutex>& robotMutex, const Params& noiseParams);
            virtual ~ArmSlamCalib();

            void LoadSimTrajectory(const std::string& fileName);
            void CreateSimTrajectory();
            void SimulateImageStep(size_t iter);

            void InitRobot(const std::string& urdf, const std::vector<std::string>& dofs, const std::string& cameraLink);
            void InitRobot(dart::dynamics::SkeletonPtr skel, const std::vector<std::string>& dofs, const std::string& cameraLink);
            void SetTrajectory(const Eigen::aligned_vector<gtsam::Vector>& trajectory);
            void SetEncoders(const Eigen::aligned_vector<gtsam::Vector>& encoders);
            void SetLandmarks(const std::vector<gtsam::Point3>& landmarks);
            void CreateSimulation(const std::string& trajFile);
            void Optimize();
            void DrawState(size_t iter, int id, const gtsam::Values& state, float r, float g, float b, float a,
                    bool drawLandmarks = true, bool drawTraj = true, bool drawObs = true, bool drawCamera = true, bool drawPointClouds = false, bool drawMarginalExt = false);
            void SimulateObservations(size_t iter);
            bool SimulateObservationsTriangulate(size_t iter);
            void SimulationStep(size_t iter);
            bool GetNewLandmarksSimulated(size_t t, const std::vector<Landmark>& visibleLandmarks, std::vector<Landmark>& newLandmarks);

            void OptimizeStep();

            void InitializeFiducial(const std::string& imageTopic, double cellSize, int numCellsX, int numCellsY);
            void InitializeAprilTags(const std::string& imageTopic, const std::string& aprtilTagsTopic);
            void InitializeFeatures(const std::string& imageTopic,
                    const std::string& rgbInfoTopic = "",
                    const std::string& depthImageTopic = "",
                    const std::string& depthInfoTopic = "");
            void InitializeUnsynchronizedDepth(const std::string& depthTopic);
            void InitializeJointRecorder(const std::string& topic);
            bool RealFrontEndStep();

            void GetVisibleLandmarks(const gtsam::Vector& q, std::vector<Landmark>& lms, const gtsam::Pose3& extrinsic);

            void UpdateViewer();

            gtsam::Pose3 GetCameraPose(const gtsam::Vector& jointAngles);
            gtsam::Pose3 GetCameraPose(const gtsam::Vector& jointAngles, const gtsam::Pose3& extr);
            gtsam::Pose3 GetCurrentExtrinsic();

            inline const Eigen::aligned_vector<gtsam::Vector>& GetTrajectory() const { return trajectory; }

            inline gtsam::Pose3 GetGroundTruthPose(size_t t)
            {
                return GetCameraPose(groundTruth.at<RobotConfig>(ConfigSymbol(t)).getQ(), GetSimExtrinsic());
            }

            inline gtsam::Pose3 GetEstimatePose(size_t t)
            {
                return GetCameraPose(currentEstimate.at<RobotConfig>(ConfigSymbol(t)).getQ(), GetCurrentExtrinsic());
            }

            inline gtsam::Pose3 GetInitialPose(size_t t)
            {
                return GetCameraPose(initialEstimate.at<RobotConfig>(ConfigSymbol(t)).getQ(), initialEstimate.at<Pose3>(ExtrinsicSymbol()));
            }

            std::shared_ptr<joint_state_recorder::JointStateRecorder> GetJointRecodrer() { return jointRecorder; }
            gtsam::Vector GetRecordedJointAngles(const ros::Time& time);
            gtsam::Vector GetSimRecordedJointAngles(const ros::Time& time);
            gtsam::Vector GetLatestJointAngles();
            CalibrationError ComputeError(const gtsam::Values& groundTruth, const gtsam::Values& current);

            inline dart::dynamics::GroupPtr GetArm() { return arm; }

            inline gtsam::Cal3_S2 GetIntrinsic() { return *calib; }

            void ShowReprojectionError(gtsam::RobotProjectionFactor<Cal3_S2>& projectionFactor, const gtsam::Vector& q, const gtsam::Point3& landmark, const gtsam::Pose3& pose, size_t index, cv::Mat& img);
            void ShowReprojectionErrors();
            void SaveGraph();

            Vector GetAugmentedEncoders(size_t iter, const gtsam::Values& state);

            void AddFactor(const gtsam::NonlinearFactor::shared_ptr& factor);
            void AddValue(const gtsam::Key& key, const gtsam::Value& value);

            void AddLandmark(Landmark& landmark);
            void AddConfig(const gtsam::Vector& encoders, size_t idx);

            bool ShouldCreateNewLandmark(const Landmark& landmark);

            void RunRansac(size_t timeA, size_t timeB);
            void UpdateLandmarkPos(size_t id, const gtsam::Point3& worldPoint);

            void CalculateCurrentErrors(std::vector<double>& errors);
            /* DISABLE_POINT_CLOUDS
            void PublishLastPointCloud();
            */

            inline dart::dynamics::SkeletonPtr GetSkeleton() { return skeleton; }

            gtsam::Vector ComputeLatestJointAngleOffsets(size_t t);
            gtsam::Vector ComputeLatestGroundTruthSimOffsets(size_t t);


            void PrintLandmarkStats(std::ofstream& stream);
            void PrintSimErrors(const std::string& dir, const std::string& postfix);

            inline gtsam::Pose3 GetSimExtrinsic() { return simExtrinsic; }

            Eigen::aligned_vector<gtsam::Vector>& GetSimTrajectory() { return simTrajectory; }

            void AddLandmarkPrior(size_t idx, const gtsam::Point3& position);
            void AddEncoderFactor(size_t idx, const gtsam::Vector& encoders);
            void AddDriftFactor(size_t idx);
            void AddVelocityFactor(size_t idx);
            void AddObservationFactor(const gtsam::Point2& observation, size_t configIdx, size_t landmarkIdx);
            /* DISABLE_POINT_CLOUDS
            void SaveStitchedPointClouds(const std::string& file);
            */
            inline double GetTime(size_t iter) const { return times.at(iter); }
            gtsam::Vector GetPerlinNoise(const gtsam::Vector& pos, const double& freq, const double& mag);
            gtsam::Vector Diff(const gtsam::Vector& q1, const gtsam::Vector& q2);
            gtsam::Vector Wrap(const gtsam::Vector& q);
            bool IsContinuous(size_t jointIndex);

            /* DISABLE_VIEWER
            inline const std::shared_ptr<aikido::rviz::InteractiveMarkerViewer>& GetViewer() { return viewer; }
            */

            inline const Params& GetParams() const { return params; }

            inline void SetSimExtrinsic(const gtsam::Pose3& pose_) { simExtrinsic = pose_; }
            inline const gtsam::Pose3& GetSimExtrinsic() const { return simExtrinsic; }

            inline Key ExtrinsicSymbol() { return Symbol('K', 0); }
            inline Key ConfigSymbol(size_t i) { return Symbol('q', i); }
            inline Key LandmarkSymbol(size_t i) { return Symbol('l', i); }

            gtsam::Values initialEstimate;
            gtsam::Values currentEstimate;
            gtsam::Values groundTruth;

        protected:
            ros::NodeHandle nh;
            ros::Publisher vizPublisher;
            ros::Publisher displayImgPublisher;
            ros::Publisher pointCloudPublisher;
            ros::Publisher latestCloudPublisher;
            ros::Publisher simJointPublisher;
            ros::Publisher simJointPublisher_groundtruth;
            ros::Publisher simEEPublisher;
            tf::TransformBroadcaster simTransformBroadcaster;

            std::shared_ptr<aikido::rviz::InteractiveMarkerViewer> viewer;
            std::shared_ptr<std::mutex> robotMutex;
            std::shared_ptr<FrontEnd> frontEnd;
            std::shared_ptr<joint_state_recorder::JointStateRecorder> jointRecorder;
            std::shared_ptr<joint_state_recorder::JointStateRecorder> simJointRecorder;
            /* DISABLE_POINT_CLOUDS
            std::vector<pcl::PointCloud<pcl::PointXYZRGB>::Ptr > pointClouds;
            pcl::PointCloud<pcl::PointXYZRGB>::Ptr lastPointCloud;
            */
            dart::simulation::WorldPtr world;
            dart::dynamics::SkeletonPtr skeleton;
            dart::dynamics::SkeletonPtr estimateSkeleton;
            dart::dynamics::GroupPtr arm;
            dart::dynamics::GroupPtr estimateArm;

            gtsam::Pose3 currentExtrinsicEstimate;
            gtsam::Pose3 simExtrinsic;

            std::vector<std::string> dofs;
            dart::dynamics::BodyNode* cameraBody;

            Eigen::aligned_vector<gtsam::Vector> trajectory;
            std::vector<double> times;
            Eigen::aligned_vector<gtsam::Vector> simTrajectory;
            Eigen::aligned_vector<gtsam::Vector> simEncoders;
            Eigen::aligned_vector<gtsam::Vector> encoders;
            Eigen::aligned_vector<gtsam::Vector> velocities;
            Eigen::aligned_vector<gtsam::Vector> simVelocities;
            std::vector<cv::Mat> images;
            std::vector<gtsam::Point3> simLandmarks;
            std::map<size_t, Landmark> landmarksObserved;
            gtsam::noiseModel::Diagonal::shared_ptr encoderNoise;
            gtsam::noiseModel::Diagonal::shared_ptr calibrationPrior;
            gtsam::noiseModel::Diagonal::shared_ptr driftNoise;
            gtsam::noiseModel::Diagonal::shared_ptr velocityNoise;
            gtsam::noiseModel::Diagonal::shared_ptr initialPoseNoise;
            gtsam::noiseModel::Robust::shared_ptr measurementNoise;
            gtsam::noiseModel::Robust::shared_ptr landmarkPrior;
            gtsam::noiseModel::mEstimator::Cauchy::shared_ptr cauchyEstimator;
            Cal3_S2::shared_ptr calib;
            Params params;
            gtsam::NonlinearFactorGraph::shared_ptr graph;
            gtsam::NonlinearFactorGraph::shared_ptr newGraph;
            size_t numObservationsThisIter;
            boost::shared_ptr<ISAM2> isam;
            size_t maxID;
            gtsam::Matrix extrinsicMarginals;
            PerlinNoise perlin;
            gtsam::Vector noiseOffset;
    };

} /* namespace gtsam */

#endif /* ARMSLAMCALIB_H_ */

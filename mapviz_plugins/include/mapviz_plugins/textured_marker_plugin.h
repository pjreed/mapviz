// *****************************************************************************
//
// Copyright (c) 2014, Southwest Research Institute® (SwRI®)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Southwest Research Institute® (SwRI®) nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// *****************************************************************************

#ifndef MAPVIZ_PLUGINS_TEXTURED_MARKER_PLUGIN_H_
#define MAPVIZ_PLUGINS_TEXTURED_MARKER_PLUGIN_H_

// C++ standard libraries
#include <string>
#include <list>
#include <map>

#include <mapviz/mapviz_plugin.h>
#include <GL/glut.h>

// QT libraries
#include <QGLWidget>
#include <QObject>
#include <QWidget>
#include <QColor>

#include <opencv2/core/core.hpp>

// ROS libraries
#include <ros/ros.h>
#include <tf/transform_datatypes.h>
#include <marti_visualization_msgs/TexturedMarker.h>
#include <marti_visualization_msgs/TexturedMarkerArray.h>


#include <mapviz/map_canvas.h>

// QT autogenerated files
#include "ui_marker_config.h"

Q_DECLARE_METATYPE(marti_visualization_msgs::TexturedMarkerConstPtr);
Q_DECLARE_METATYPE(marti_visualization_msgs::TexturedMarkerArrayConstPtr);

namespace mapviz_plugins
{
  class TexturedMarkerPlugin : public mapviz::MapvizPlugin
  {
    Q_OBJECT

  public:
    TexturedMarkerPlugin();
    virtual ~TexturedMarkerPlugin();

    bool Initialize(QGLWidget* canvas);
    void Shutdown() {}

    void Draw(double x, double y, double scale);

    void Transform();

    void LoadConfig(const YAML::Node& node, const std::string& path);
    void SaveConfig(YAML::Emitter& emitter, const std::string& path);

    QWidget* GetConfigWidget(QWidget* parent);

  Q_SIGNALS:
    void MarkerReceived(const marti_visualization_msgs::TexturedMarkerConstPtr marker);
    void MarkersReceived(const marti_visualization_msgs::TexturedMarkerArrayConstPtr markers);

  protected:
    void PrintError(const std::string& message);
    void PrintInfo(const std::string& message);
    void PrintWarning(const std::string& message);

  protected Q_SLOTS:
    void SelectTopic();
    void TopicEdited();
    void ProcessMarker(const marti_visualization_msgs::TexturedMarkerConstPtr marker);
    void ProcessMarkers(const marti_visualization_msgs::TexturedMarkerArrayConstPtr markers);

  private:
    struct MarkerData
    {
      ros::Time stamp;
      ros::Time expire_time;

      float alpha_;

      std::vector<uint8_t> texture_;
      int32_t texture_id_;
      int32_t texture_size_;
      float texture_x_;
      float texture_y_;
      
      std::string encoding_;
      
      std::vector<tf::Vector3> quad_;
      std::vector<tf::Vector3> transformed_quad_;
      
      bool transformed;
    };

    Ui::marker_config ui_;
    QWidget* config_widget_;

    std::string topic_;

    ros::Subscriber marker_sub_;
    bool has_message_;

    std::map<std::string, std::map<int, MarkerData> > markers_;

    bool is_marker_array_;

    void ProcessMarker(const marti_visualization_msgs::TexturedMarker& marker);

    void MarkerCallback(const marti_visualization_msgs::TexturedMarkerConstPtr marker);

    void MarkerArrayCallback(
      const marti_visualization_msgs::TexturedMarkerArrayConstPtr markers);
  };
}

#endif  // MAPVIZ_PLUGINS_TEXTURED_MARKER_PLUGIN_H_

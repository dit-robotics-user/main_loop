<launch>
   <node name = "main_frank" pkg = "main_loop" type="main_frank" output="screen" respawn="true">

      <param  name="rx0" type="int" value="1489"/>
      <param  name="rx1" type="int" value="5656"/>
      <param  name="rx2" type="int" value="5"/>
      <param  name="rx3" type="int" value="6"/>

      <param  name="goal_pos_x" type="int" value="1000"/>
      <param  name="goal_pos_y" type="int" value="1000"/>
      <param  name="enemy1_x" type="int" value="1300"/>
      <param  name="enemy1_y" type="int" value="1800"/>
      <param  name="enemy2_x" type="int" value="5000"/>
      <param  name="enemy2_y" type="int" value="5000"/>
      <param  name="ally_x" type="int" value="1300"/>
      <param  name="ally_y" type="int" value="1300"/>

   </node>	

   <node pkg="main_loop" type="goap_main_v1.py" name="goap_main_v1" respawn="true" >
   </node>

   <node pkg="main_loop" type="path_planning_" name="path_planning_" output="log" respawn="true">
   </node>


</launch>

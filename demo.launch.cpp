<launch>
   <node name = "main_frank" pkg = "main_loop" type="main_frank" output="screen">

      <param  name="rx0" type="int" value="1489"/>
      <param  name="rx1" type="int" value="56"/>
      <param  name="rx2" type="int" value="56"/>
      <param  name="rx3" type="int" value="6"/>

      <param  name="goal_pos_x" type="int" value="1489"/>
      <param  name="goal_pos_y" type="int" value="56"/>
      <param  name="enemy1_x" type="int" value="56"/>
      <param  name="enemy1_y" type="int" value="6"/>
      <param  name="enemy2_x" type="int" value="6"/>
      <param  name="enemy2_y" type="int" value="6"/>
      <param  name="ally_x" type="int" value="6"/>
      <param  name="ally_y" type="int" value="6"/>

   </node>	

   <node pkg="main_loop" type="goap_main_v1.py" name="goap_main_v1" respawn="true" >
   </node>

   <node pkg="main_loop" type="path_planning_" name="path_planning_" output="log" respawn="true">
   </node>

   <node pkg="serial_port" type="serial_port_node" name="serial_port_node" output="log" respawn="true">
   </node>

   <node pkg="serial_port" type="serial_port_node_2" name="serial_port_node_2" output="log" respawn="true">
   </node>

</launch>

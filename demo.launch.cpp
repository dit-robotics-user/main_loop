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
</launch>
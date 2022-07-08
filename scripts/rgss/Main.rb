#==============================================================================
# ** Main
#------------------------------------------------------------------------------
#  After defining each class, actual processing begins here.
#==============================================================================

at_exit do
  Wallpaper.reset
  save unless $game_switches[99] || ($game_system.map_interpreter.running? || !$scene.is_a?(Scene_Map))
end

module Audio
  class << self
    def method_missing(*args)
    end
  end
end

def run_fmod_test
  result, s = FMOD::Studio::System.new(512, 0, 0)
  puts "result: #{result} system: #{s}"
  puts s.is_valid

  result, bank = s.load_bank_file("Master.bank", FMOD::Studio::LoadBankFlags::NONBLOCKING)
  result, strbank = s.load_bank_file("Master.strings.bank", 0)
  puts bank.is_valid.inspect
  puts bank.get_path.inspect
  puts strbank.is_valid.inspect
  puts strbank.get_path.inspect

  result, count = strbank.get_string_count
  count.times do |i|
    puts strbank.get_string_info(i).inspect
  end

  result, list = bank.get_vca_list
  puts list.inspect
  list.each_with_index do |v, i|
    puts "index: #{i} vca: #{v} valid: #{v.is_valid}"
    puts "guid: #{v.get_id[1].inspect}"
    puts v.get_path.inspect
    puts v.get_volume.inspect
  end

  result, list2 = bank.get_bus_list
  puts list2.inspect
  list2.each_with_index do |b, i|
    puts "index: #{i} bus: #{b} valid: #{b.is_valid}"
    puts "guid: #{b.get_id[1].inspect}"
    puts b.get_path.inspect
    puts b.get_volume.inspect
    puts b.get_paused.inspect
    puts b.get_cpu_usage.inspect
    puts b.get_memory_usage.inspect
  end

  def dump_parameter(d)
    puts "param: #{d}"
    puts "name: #{d.name}"
    puts "id: #{d.id.inspect}"
    puts "min: #{d.minimum} max: #{d.maximum}"
    puts "default: #{d.default_value}"
    puts "type: #{d.type}"
    puts "flags: #{d.flags}"
    puts "guid: #{d.guid}"
  end

  result, list3 = bank.get_event_list
  puts list3.inspect
  list3.each_with_index do |b, i|
    puts "index: #{i} bus: #{b} valid: #{b.is_valid}"
    puts "guid: #{b.get_id[1].inspect}"
    puts b.get_path.inspect
    result, count = b.get_parameter_description_count
    count.times do |j|
      result, d = b.get_parameter_description_by_index(j)
      dump_parameter(d)
      #puts b.get_parameter_label_by_index(j, 0.0).inspect
      #puts b.get_parameter_label_by_name(d.name, 0.0).inspect
      #puts b.get_parameter_label_by_id(d.id, 0.0).inspect
    end
    result, count = b.get_user_property_count
    count.times do |j|
      result, d = b.get_user_property_by_index(j)
      puts "user property: #{d}"
      puts "name: #{d.name}"
      puts "type: #{d.type}"
      puts "value: #{d.value}"
    end
    puts b.get_length.inspect
    puts b.get_min_max.inspect
    puts b.get_min_max.inspect
    puts b.is_snapshot.inspect
    puts b.is_oneshot.inspect
    puts b.is_stream.inspect
    puts b.is_3d.inspect
    puts b.is_doppler_enabled.inspect
    puts b.has_sustain_point.inspect
    puts b.get_instance_count.inspect
    puts b.load_sample_data.inspect
    puts b.get_sample_loading_state.inspect
    puts b.unload_sample_data.inspect
    puts b.release_all_instances.inspect
  end
end

run_fmod_test()
GC.start

begin
  $console = Graphics.fullscreen
  Graphics.frame_rate = 60
  Font.default_size = 20

  # Load persistent data
  Persistent.load

  # Prepare for transition
  Graphics.freeze
  $demo = false
  $GDC = false
  # Make scene object (title screen)
  $scene = Scene_Title.new
  Oneshot.allow_exit false
  Oneshot.exiting false

  #  x = Oneshot.textinput("Foo Bar")
  #  print("#{x}")

  # Call main method as long as $scene is effective
  while $scene != nil
    $scene.main
  end
  # Fade out
  Oneshot.exiting true
  Graphics.transition(20)

  if Journal.active?
    Journal.set ""
  end

  Oneshot.allow_exit true
rescue Errno::ENOENT
  # Supplement Errno::ENOENT exception
  # If unable to open file, display message and end
  filename = $!.message.sub("No such file or directory - ", "")
  print("Unable to find file #{filename}.")
end

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
  puts bank.is_valid.to_s
  puts bank.get_path.to_s
  puts strbank.is_valid.to_s
  puts strbank.get_path.to_s

  result, count = strbank.get_string_count
  count.times do |i|
    puts strbank.get_string_info(i).to_s
  end

  result, list = bank.get_vca_list
  puts list.to_s
  list.each_with_index do |v, i|
    puts "index: #{i} vca: #{v} valid: #{v.is_valid}"
    puts "guid: #{v.get_id[1].inspect}"
    puts v.get_path.to_s
    puts v.get_volume.to_s
  end

  result, list2 = bank.get_bus_list
  puts list2.to_s
  list2.each_with_index do |b, i|
    puts "index: #{i} bus: #{b} valid: #{b.is_valid}"
    puts "guid: #{b.get_id[1].inspect}"
    puts b.get_path.to_s
    puts b.get_volume.to_s
    puts b.get_paused.to_s
    puts b.get_port_index.to_s
    puts b.get_cpu_usage.to_s
    puts b.get_memory_usage.to_s
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
  puts list3.to_s
  list3.each_with_index do |b, i|
    puts "index: #{i} bus: #{b} valid: #{b.is_valid}"
    puts "guid: #{b.get_id[1].inspect}"
    puts b.get_path.to_s
    result, count = b.get_parameter_description_count
    count.times do |j|
      result, d = b.get_parameter_description_by_index(j)
      dump_parameter(d)
      #puts b.get_parameter_label_by_index(j, 0.0).to_s
      #puts b.get_parameter_label_by_name(d.name, 0.0).to_s
      #puts b.get_parameter_label_by_id(d.id, 0.0).to_s
    end
    result, count = b.get_user_property_count
    count.times do |j|
      result, d = b.get_user_property_by_index(j)
      puts "user property: #{d}"
      puts "name: #{d.name}"
      puts "type: #{d.type}"
      puts "value: #{d.value}"
    end
    puts b.get_length.to_s
    puts b.get_min_max.to_s
    puts b.get_min_max.to_s
    puts b.is_snapshot.to_s
    puts b.is_oneshot.to_s
    puts b.is_stream.to_s
    puts b.is_3d.to_s
    puts b.is_doppler_enabled.to_s
    puts b.has_sustain_point.to_s
    puts b.get_instance_count.to_s
    puts b.load_sample_data.to_s
    puts b.get_sample_loading_state.to_s
    puts b.unload_sample_data.to_s
    puts b.release_all_instances.to_s
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
